begin
  require "socket"
  require "tmpdir"
  require "test/unit"
rescue LoadError
end

class TestSocket < Test::Unit::TestCase
  def test_socket_new
    s = Socket.new(:INET, :STREAM)
    assert_kind_of(Socket, s)
  end

  def test_unpack_sockaddr
    sockaddr_in = Socket.sockaddr_in(80, "")
    assert_raise(ArgumentError) { Socket.unpack_sockaddr_un(sockaddr_in) }
    sockaddr_un = Socket.sockaddr_un("/tmp/s")
    assert_raise(ArgumentError) { Socket.unpack_sockaddr_in(sockaddr_un) }
    assert_raise(ArgumentError) { Socket.unpack_sockaddr_in("") }
    assert_raise(ArgumentError) { Socket.unpack_sockaddr_un("") }
  end if Socket.respond_to?(:sockaddr_un)

  def test_sysaccept
    serv = Socket.new(Socket::AF_INET, Socket::SOCK_STREAM, 0)
    serv.bind(Socket.sockaddr_in(0, "127.0.0.1"))
    serv.listen 5
    c = Socket.new(Socket::AF_INET, Socket::SOCK_STREAM, 0)
    c.connect(serv.getsockname)
    fd, peeraddr = serv.sysaccept
    assert_equal(c.getsockname, peeraddr.to_sockaddr)
  ensure
    serv.close if serv
    c.close if c
    IO.for_fd(fd).close if fd
  end

  def test_initialize
    Socket.open(Socket::AF_INET, Socket::SOCK_STREAM, 0) {|s|
      addr = s.getsockname
      assert_nothing_raised { Socket.unpack_sockaddr_in(addr) }
      assert_raise(ArgumentError) { Socket.unpack_sockaddr_un(addr) }
    }
    Socket.open("AF_INET", "SOCK_STREAM", 0) {|s|
      addr = s.getsockname
      assert_nothing_raised { Socket.unpack_sockaddr_in(addr) }
      assert_raise(ArgumentError) { Socket.unpack_sockaddr_un(addr) }
    }
    Socket.open(:AF_INET, :SOCK_STREAM, 0) {|s|
      addr = s.getsockname
      assert_nothing_raised { Socket.unpack_sockaddr_in(addr) }
      assert_raise(ArgumentError) { Socket.unpack_sockaddr_un(addr) }
    }
  end

  def test_getaddrinfo
    # This should not send a DNS query because AF_UNIX.
    assert_raise(SocketError) { Socket.getaddrinfo("www.kame.net", 80, "AF_UNIX") }
  end

  def test_getnameinfo
    assert_raise(SocketError) { Socket.getnameinfo(["AF_UNIX", 80, "0.0.0.0"]) }
  end

  def test_ip_address_list
    begin
      list = Socket.ip_address_list
    rescue NotImplementedError
      return
    end
    list.each {|ai|
      assert_instance_of(AddrInfo, ai)
      assert(ai.ip?)
    }
  end

  def tcp_unspecified_to_loopback(addrinfo)
    if addrinfo.ipv4? && addrinfo.ip_address == "0.0.0.0"
      AddrInfo.tcp("127.0.0.1", addrinfo.ip_port)
    elsif addrinfo.ipv6? && addrinfo.ipv6_unspecified?
      AddrInfo.tcp("::1", addrinfo.ip_port)
    elsif addrinfo.ipv6? && (ai = addrinfo.ipv6_to_ipv4) && ai.ip_address == "0.0.0.0"
      AddrInfo.tcp("127.0.0.1", addrinfo.ip_port)
    else
      addrinfo
    end
  end

  def test_tcp
    TCPServer.open(0) {|serv|
      addr = serv.local_address
      addr = tcp_unspecified_to_loopback(addr)
      addr.connect {|s1|
        s2 = serv.accept
        begin
          assert_equal(s2.remote_address.ip_unpack, s1.local_address.ip_unpack)
        ensure
          s2.close
        end
      }
    }
  end

  def random_port
    # IANA suggests dynamic port for 49152 to 65535
    # http://www.iana.org/assignments/port-numbers
    49152 + rand(65535-49152+1)
  end

  def test_tcp_server_sockets
    port = random_port
    begin
      sockets = Socket.tcp_server_sockets(port)
    rescue Errno::EADDRINUSE
      return # not test failure
    end
    begin
      sockets.each {|s|
        assert_equal(port, s.local_address.ip_port)
      }
    ensure
      sockets.each {|s|
        s.close
      }
    end
  end

  def test_tcp_server_sockets_port0
    sockets = Socket.tcp_server_sockets(0)
    ports = sockets.map {|s| s.local_address.ip_port }
    the_port = ports.first
    ports.each {|port|
      assert_equal(the_port, port)
    }
  ensure
    if sockets
      sockets.each {|s|
        s.close
      }
    end
  end

  if defined? UNIXSocket
    def test_unix
      Dir.mktmpdir {|tmpdir|
        path = "#{tmpdir}/sock"
        UNIXServer.open(path) {|serv|
          Socket.unix(path) {|s1|
            s2 = serv.accept
            begin
              s2raddr = s2.remote_address
              s1laddr = s1.local_address
              assert(s2raddr.to_sockaddr.empty? ||
                     s1laddr.to_sockaddr.empty? ||
                     s2raddr.unix_path == s1laddr.unix_path)
            ensure
              s2.close
            end
          }
        }
      }
    end

    def test_unix_server_socket
      Dir.mktmpdir {|tmpdir|
        path = "#{tmpdir}/sock"
        2.times {
          serv = Socket.unix_server_socket(path)
          begin
            assert_kind_of(Socket, serv)
            assert(File.socket?(path))
            assert_equal(path, serv.local_address.unix_path)
          ensure
            serv.close
          end
        }
      }
    end

    def test_accept_loop_with_unix
      Dir.mktmpdir {|tmpdir|
        tcp_servers = []
        clients = []
        accepted = []
        begin
          tcp_servers = Socket.tcp_server_sockets(0)
          unix_server = Socket.unix_server_socket("#{tmpdir}/sock")
          tcp_servers.each {|s|
            addr = tcp_unspecified_to_loopback(s.local_address)
            clients << addr.connect
          }
          clients << unix_server.local_address.connect
          Socket.accept_loop(tcp_servers, unix_server) {|s|
            accepted << s
            break if clients.length == accepted.length
          }
          assert_equal(clients.length, accepted.length)
        ensure
          tcp_servers.each {|s| s.close if !s.closed?  }
          unix_server.close if !unix_server.closed?
          clients.each {|s| s.close if !s.closed?  }
          accepted.each {|s| s.close if !s.closed?  }
        end
      }
    end
  end

  def test_accept_loop
    servers = []
    begin
      servers = Socket.tcp_server_sockets(0)
      port = servers[0].local_address.ip_port
      Socket.tcp("localhost", port) {|s1|
        Socket.accept_loop(servers) {|s2, client_ai|
          begin
            assert_equal(s1.local_address.ip_unpack, client_ai.ip_unpack)
          ensure
            s2.close
          end
          break
        }
      }
    ensure
      servers.each {|s| s.close if !s.closed?  }
    end

  end

end if defined?(Socket)
