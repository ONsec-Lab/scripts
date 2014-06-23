require 'socket'

server = TCPServer.new 8000 
loop do
  Thread.start(server.accept) do |client|
	puts "New client connected"
	data = ""
	client.puts("220 xxe-ftp-server")
	loop {
		req = client.gets()
		puts "< "+req
		#if req.include? "LIST"
		#	client.puts("drwxrwxrwx 1 owner group          1 Feb 21 04:37 test")
		#	client.puts("150 Opening BINARY mode data connection for /bin/ls")
		#	client.puts("226 Transfer complete.")
		#els
		if req.include? "USER"
			client.puts("331 password please - version check")
		#elsif req.include? "PORT"
		#	puts "! PORT received"
		#	puts "> 200 PORT command ok"
		#	client.puts("200 PORT command ok")
		else
			puts "> 230 more data please!"
			client.puts("230 more data please!")
		end
	}
  end
end
