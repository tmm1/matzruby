#! /usr/bin/ruby

defined?(ChangeLog) or ChangeLog = "ChangeLog"
class << ChangeLog
  require 'date'

  module Reader
    CONFLICT_BEGIN = "\n<<<<<<<".freeze
    CONFLICT_SEP = "\n=======".freeze
    CONFLICT_END = "\n>>>>>>>".freeze
    LOGENTRY_PAT = /([A-Z][a-z]{2} [A-Z][a-z]{2} [ 1-9]\d \d\d:\d\d:\d\d \d{4})  .*>(?:\n(?:\t.*)?)+\n\n/

    def split_entry(ents)
      ents.gsub!(/[ \t]+$/, '')
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

  module Writer
    def merge(working, merged)
      until working.empty? or merged.empty?
        later = merged.first[0] > working.first[0] ? merged : working
        self.print(later.shift[1])
      end
      working.each {|i| self.print i[1]}
      merged.each {|i| self.print i[1]}
    end
  end

  def resolve
    puts "resolving #{self}"
    newlog = self + ".new"
    open(self) do |fi|
      fi.extend(Reader)
      open(newlog, "wb") do |fo|
        fo.extend(Writer)
        fi.each_hunk do |before, working, merged|
          fo.print before
          fo.merge(working, merged) if working
        end and newlog
      end
    end
  end
end

if $0 == __FILE__
  newlog = ChangeLog.resolve or abort
  puts "resolved, see #{newlog}"
end
