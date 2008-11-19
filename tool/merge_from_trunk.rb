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
  IO.foreach("|svn merge --accept postpone #{TRUNK}@#{old_revision} #{TRUNK}@#{new_revision} .") do |line|
    puts line
    conflicts << line[5..-2] if /^C/ =~ line
  end
  File.open(".merged-trunk-revision", "wb") {|f| f.puts new_revision}
end
if conflicts.include?(ChangeLog)
  $:.unshift(File.dirname(__FILE__))
  require "resolve_changelog"
  if newlog = ChangeLog.resolve
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
