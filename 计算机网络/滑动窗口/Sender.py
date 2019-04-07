import os
import sys
import getopt

import Checksum
import BasicSender
import threading
import time

'''
This is a skeleton sender class. Create a fantastic transport protocol here.
'''
class Sender(BasicSender.BasicSender):
    def __init__(self, dest, port, filename, debug=False):
        super(Sender, self).__init__(dest, port, filename, debug)
        self.rc = True

        # width
        self.width = 5

        # mark_list
        self.mark_list = [False for i in range(self.width)]

        # timeout
        self.timeout = 0.5000

        self.seqno = 0

        self.Max = 0


    # Main sending loop.
    def start(self):
        if self.infile == sys.stdin:
            seqno = 0
            msg_type = None
            while not msg_type == 'end':
                msg = raw_input("Message:")

                msg_type = 'data'
                if seqno == 0:
                    msg_type = 'start'
                elif msg == "done":
                    msg_type = 'end'

                packet = self.make_packet(msg_type, seqno, msg)
                self.send(packet)

                response = self.receive(0.5)
                check = 0
                if response != None:
                    check = int(response.split('|')[1])

                while (not Checksum.validate_checksum(response)) or (check >= seqno+1):
                    self.send(packet)
                    response = self.receive(0.5)

                seqno += 1

        # send file in any mode
        else:

            size = os.path.getsize(self.infile.name)
            if size % 1472 == 0:
                size //= 1472
            else:
                size = size//1472 + 1

            # msg_list
            msg_list = list()

            # limit
            limit = 0
            false = 0

            b = 0

            while self.Max < size:

                th = threading.Thread(target=self.rec_list)

                # packets are completed
                for i in range(false, self.width):
                    msg_list.append(self.infile.read(1472))
                    msg_type = 'data'
                    if self.seqno+i == 0:
                        msg_type = 'start'
                    elif self.seqno + i + 1 == size:
                        msg_type = 'end'

                        self.width = i + 1

                        msg_list[i] = self.make_packet(msg_type, self.seqno + i, msg_list[i])

                        break

                    msg_list[i] = self.make_packet(msg_type, self.seqno+i, msg_list[i])

                # receive

                th.start()

                # send
                for i in range(self.width):
                    if self.Max > self.seqno + i:
                        continue
                    self.send(msg_list[i])

                # waiting
                while self.rc:
                    time.sleep(0.02)

                self.rc = True

                # work

                for i in range(self.width):
                    if self.mark_list[(i * -1) - 1]:
                        limit = self.width - i
                        break
                    limit = 0
                false = self.width - limit

                b = limit/self.width
                if b > 0.95:
                    self.timeout /= 2
                    self.timeout += 0.02
                    self.width += 10
                elif b > 0.85:
                    self.timeout /= 3
                    self.timeout *= 2
                    self.timeout += 0.01
                    self.width += 6
                elif b < 0.15:
                    self.timeout += 0.1
                    self.timeout = min(0.500, self.timeout)
                    self.width = (self.width + 1)//2
                    self.width = max(5, self.width)

                self.mark_list = self.mark_list[limit:] + [False for s in range(self.width - limit, self.width)]
                msg_list = msg_list[limit:]
                self.seqno += limit


            self.infile.close()

    def rec_list(self):
        for i in range(self.width):
            response = self.receive(self.timeout)
            if response == None or Checksum.validate_checksum(response) == False:
                continue
            else:
                check = int(response.split('|')[1])
                if self.Max < check:
                    self.Max = check
                if  self.seqno < check <= self.seqno + self.width:
                    self.mark_list[check - self.seqno - 1] = True
        self.rc = False
        return

'''
This will be run if you run this script from the command line. You should not
change any of this; the grader may rely on the behavior here to test your
submission.
'''
if __name__ == "__main__":
    def usage():
        print "BEARS-TP Sender"
        print "-f FILE | --file=FILE The file to transfer; if empty reads from STDIN"
        print "-p PORT | --port=PORT The destination port, defaults to 33122"
        print "-a ADDRESS | --address=ADDRESS The receiver address or hostname, defaults to localhost"
        print "-d | --debug Print debug messages"
        print "-h | --help Print this usage message"

    try:
        opts, args = getopt.getopt(sys.argv[1:],
                               "f:p:a:d", ["file=", "port=", "address=", "debug="])
    except:
        usage()
        exit()

    port = 33122
    dest = "localhost"
    filename = None
    debug = False

    for o,a in opts:
        if o in ("-f", "--file="):
            filename = a
        elif o in ("-p", "--port="):
            port = int(a)
        elif o in ("-a", "--address="):
            dest = a
        elif o in ("-d", "--debug="):
            debug = True

    s = Sender(dest,port,filename,debug)
    try:
        s.start()
    except (KeyboardInterrupt, SystemExit):
        exit()
