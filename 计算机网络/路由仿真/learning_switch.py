from sim.api import *
from sim.basics import *
import time

class LearningSwitch (Entity):

  def __init__(self):
    self.r_table = dict()

  def handle_rx (self, packet, port):
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
      head = source[:limit]
      tail = source[limit + 2:]
      self.r_table[head] = port
      if (self.r_table.has_key(tail)):
        self.send(packet, self.r_table[tail])
      else:
        self.send(packet, port, flood=True)
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
      tail = source[limit + 2:]
      self.r_table[tail] = port
      self.send(packet, self.r_table[head])
    else:
      self.send(packet, port, flood=True)