#!/usr/bin/ruby

ENV["LC_MESSAGES"] = ENV["LC_ALL"] = "C"
ChangeLog = "ChangeLog"

if ARGV[0] == "--commit"
  ARGV.shift
  commit = true
end
TRUNK = ARGV[0] || ENV["RUBY_SVN_TRUNK"] || "svn+ssh://svn@ci.ruby-lang.org/ruby/trunk"
new_revision = `svn info #{TRUNK}@HEAD`[/^Revision:\s*(\S+)/, 1]
conflicts = []
old_revision = IO.read(".merged-trunk-revision").chomp
unless old_revision == new_revision
  puts "merging r#{old_revision}:#{new_revision}"
  IO.foreach("|svn merge #{TRUNK}@#{old_revision} #{TRUNK}@#{new_revision} .") do |line|
    puts line
    conflicts << line[5..-2] if /^C/ =~ line
  end
  File.open(".merged-trunk-revision", "wb") {|f| f.puts new_revision}
end
if conflicts.include?(ChangeLog)
  require 'date'
  puts "resolving #{ChangeLog}"
  CONFLICT_BEGIN = "\n<<<<<<<"
  CONFLICT_SEP = "\n======="
  CONFLICT_END = "\n>>>>>>>"
  LOGENTRY_PAT = /([A-Z][a-z]{2} [A-Z][a-z]{2} [ 1-9]\d \d\d:\d\d:\d\d \d{4})  .*>(?:\n(?:\t.*)?)+\n\n/
  newlog = ChangeLog + ".new"
  if open(ChangeLog) do |fi|
      class << fi
        def split_entry(ents)
          results = []
          unless ents.empty?
            ents.scan(/\G(#{LOGENTRY_PAT}|[.\n]+\z)/o) do |ent, time|
              time or return
              results << [(time and DateTime.parse(time)), ent]
            end
          end
          results
        end
        def each_hunk
          while line = self.gets
            if /^<<<<<<</ =~ line
              before = ""
            else
              before = self.gets(CONFLICT_BEGIN)
              before[0, 0] = line
              unless before.chomp!(CONFLICT_BEGIN)
                yield before, nil, nil
                break
              end
              self.gets
            end
            working = self.gets(CONFLICT_SEP) or return
            working.chomp!(CONFLICT_SEP[1..-1]) and self.gets
            working = split_entry(working) or return
            merged = self.gets(CONFLICT_END) or return
            merged.chomp!(CONFLICT_END[1..-1]) and self.gets
            merged = split_entry(merged) or return
            yield before, working, merged
          end
          true
        end
      end
      open(newlog, "wb") do |fo|
        fi.each_hunk do |before, working, merged|
          fo.print before
          if working
            until working.empty? or merged.empty?
              fo.print((merged.first[0] > working.first[0] ? merged : working).shift[1])
            end
            working.each {|i| fo.print i[1]}
            merged.each {|i| fo.print i[1]}
          end
        end
      end
    end
    File.rename(newlog, ChangeLog)
    if system("svn", "resolved", ChangeLog)
      conflicts.delete(ChangeLog)
    end
  end
end
unless conflicts.empty?
  abort "conflicted #{conflicts.size}"
end
curr_revision = `svn cat .merged-trunk-revision`.chomp
merge = ["svn", "ci", "-m", "* merged from trunk r#{curr_revision}:#{new_revision}.", "."]
if commit
  system(*merge)
else
  puts merge.map{|s|/\s/=~s ? "'#{s}'" : s}.join(" ")
end
