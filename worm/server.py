
from BaseHTTPServer import BaseHTTPRequestHandler,HTTPServer
import os

PORT_NUMBER = 8080

lastImage = None
accumKeys = "";

class myHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        print "got GET", self.path
        
        if self.path.startswith("/lastimage.png"):
            if lastImage is not None:
                self.send_response(200)
                self.send_header('Content-type','image/png')
                self.end_headers()
                self.wfile.write(lastImage)
                print "sent image", len(lastImage)
        elif self.path.startswith("/keys.txt"):
            self.send_response(200)
            self.send_header('Content-type','text/html')
            self.end_headers()
            self.wfile.write("<html><head></head><body>" + accumKeys + "</body></html>")
            print "sent text", accumKeys
        else:
            filename = self.path[1:]
            if filename == "/":
                filename == "index.html"
            if os.path.exists(filename):
                print "getting file", filename
                with open(filename, "rb") as f:
                    data = f.read()
                self.send_response(200)
                self.send_header('Content-type','text/html')
                self.end_headers()
                self.wfile.write(data)
            
            
    
    def do_POST(self):
        print "got POST", self.path
        data = self.rfile.read()
        if self.path.startswith("/screen"):
            global lastImage
            lastImage = data
            print "saved image", len(data)
        elif self.path.startswith("/keys"):
            global accumKeys
            accumKeys += data
            print "got keys", data
        

def main():
    server = HTTPServer(('', PORT_NUMBER), myHandler)
    print 'Started httpserver on port ' , PORT_NUMBER

    server.serve_forever()


main()