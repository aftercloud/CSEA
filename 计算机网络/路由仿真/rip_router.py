from sim.api import *
from sim.basics import *

'''
Create your RIP router in this file.
'''
class RIPRouter (Entity):
    def __init__(self):
        self.r_table = dict()
        self.old_packet = ""

    def handle_rx (self, packet, port):
        selfName = str(self)
        ind = 0
        for x in selfName:
            if x == ' ':
                selfName = selfName[ind+1:]
                break
            ind += 1
        selfName = selfName[:len(selfName)-1]
        if isinstance(packet, DiscoveryPacket):
            # packet_str
            ptr = str(packet)[22:]
            # index
            ind = 0
            # source
            source = ''
            for tmp in ptr[:]:
                if tmp == '-':
                    source = ptr[:ind]
                ind += 1
            if ptr[len(ptr)-5:] == 'alse>':
                for x in self.r_table:
                    for y in self.r_table[x]:
                        if y[0] == port:
                            y[1] = 10000
                            break
                return
            tmp = list()
            temp = list()
            temp.append(port)
            temp.append(1)
            tmp.append(temp)
            if source in self.r_table:
                self.r_table[source].append(temp)
            else:
                self.r_table[source] = tmp
            routing = RoutingUpdate()
            for x in self.r_table:
                distance = 10000
                for y in self.r_table[x]:
                    if distance > y[1]:
                        distance = y[1]
                routing.add_destination(x, distance)
            self.send(routing, -1, True)
        elif str(packet)[0:2] == "<R":
            # if flood or not
            mark = False
            List = packet.all_dests()
            for x in List:
                if x == selfName:
                    continue
                if x in self.r_table:
                    isHave = False
                    for y in self.r_table[x]:
                        if y[0] == port:
                            isHave = True
                    if not isHave:
                        self.r_table[x].append([port, packet.get_distance(x)+1])
                        continue
                    for y in self.r_table[x]:
                        if port == y[0] and packet.get_distance(x)+1 < y[1]:
                            mark = True
                            self.r_table[x].remove(y)
                            self.r_table[x].append([port, packet.get_distance(x)+1])
                else:
                    mark = True
                    self.r_table[x] = [[port, packet.get_distance(x)+1]]
            if mark:
                routing = RoutingUpdate()
                for x in self.r_table:
                    distance = 10000
                    for y in self.r_table[x]:
                        if distance > y[1]:
                            distance = y[1]
                    routing.add_destination(x, distance)
                self.send(routing, -1, True)
        else:
            s_packet = str(packet)
            judge = s_packet[1:5]
            if judge == 'Ping':
                source = s_packet[6:]
                limit = 0
                for i in range(len(source)):
                    if source[i] == ' ':
                        limit = i
                        break
                source = source[:limit]
                for i in range(len(source)):
                    if source[i] == '-':
                        limit = i
                        break
                tail = source[limit + 2:]
                dests = self.r_table[tail]
                distance = 10000
                t_port = 0
                if self.old_packet == str(packet)[:len(str(packet))-5]:
                    self.old_packet = ""
                    for x in dests:
                        if x[0] == port:
                            x[1] = 10000
                else:
                    self.old_packet = str(packet)[:len(str(packet))-5]
                for x in dests:
                    if distance > x[1]:
                        distance = x[1]
                        t_port = x[0]
                if distance >= 10000:
                    return
                self.send(packet, t_port)
            elif judge == 'Pong':
                source = s_packet[12:]
                limit = 0
                for i in range(len(source)):
                    if source[i] == ' ':
                        limit = i
                        break
                source = source[:limit]
                for i in range(len(source)):
                    if source[i] == '-':
                        limit = i
                        break
                head = source[:limit]
                dests = self.r_table[head]
                distance = 10000
                t_port = 0
                if self.old_packet == str(packet)[:len(str(packet))-5]:
                    self.old_packet = ""
                    for x in dests:
                        if x[0] == port:
                            x[1] = 10000
                else:
                    self.old_packet = str(packet)[:len(str(packet))-5]
                for x in dests:
                    if distance > x[1]:
                        distance = x[1]
                        t_port = x[0]
                if distance >= 10000:
                    return
                self.send(packet, t_port)