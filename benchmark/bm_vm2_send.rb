class C
  def m
  end
end

o = C.new

i=0
while i<6000000 # benchmark loop 2
  i+=1
  o.__send__ :m
end
