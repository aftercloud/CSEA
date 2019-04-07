# -*- coding:utf-8 -*-
import socket as sk
import Tkinter as tk
import tkMessageBox as mb
import ttk
import time
import threading as td

# 端口号累计
count = 0

# 全局端口号
port = 21578

# 全局端口名
mark = ''

# 主线程socket状态转移
stop = False

# 登陆
def sign():
    str1 = entry1.get()
    str2 = entry2.get()
    str3 = entry3.get()
    res ='01#'+str1+'#'+str2+'#'+str3+'#'
    udpCliSock.sendto(res,ADDR)
    data,addr = udpCliSock.recvfrom(1024)
    if data=='01:02':
        mb.showerror('错误','确认密码错误')
    else:
        res = '02#' + str1 + '#' + str2 + '#'
        udpCliSock.sendto(res,ADDR)
        data,addr = udpCliSock.recvfrom(1024)
        if data=='02:01':
            mb.showinfo('提示', '登陆成功')
            number = open('../log/check.se','w')
            number.write('1')
            number.close()
            number = open('../log/login.se', 'r')
            data = number.read()
            number.close()
            data = data.split()
            if str1 not in data:
                number = open('../log/login.se', 'a')
                data = ' '+str1
                number.write(data)
                number.close()
            root.destroy()
        elif data=='02:02':
            mb.showerror('错误','密码错误')
        elif data=='02:04':
            mb.showerror('错误','用户已登陆')

# 添加好友
def inse():
    def subm():
        data = tentry.get()
        number = open('../log/friends.se', 'a')
        data = ' '+data
        number.write(data)
        number.close()
        number = open('../log/friends.se', 'r')
        data = number.read()
        number.close()
        data = data.split()
        combo['values'] = tuple(data)
        temp.destroy()

    temp = tk.Tk()
    temp.title('If')
    temp.iconbitmap('../icon/aowu.ico')
    temp.geometry('%dx%d+%d+%d' % (186, 86, (temp.winfo_screenwidth() - 206 ) / 2 , (temp.winfo_screenheight() - 122) / 2 ))
    temp.maxsize(186,86)
    temp.minsize(186,86)
    tentry = ttk.Entry(temp,width=20)
    tentry.place(x=21,y=17)
    tbutton = ttk.Button(temp,width=20,text='添加好友',command=subm)
    tbutton.place(x=20,y=46)
    temp.mainloop()

# 开始聊天
def begin():
    global count
    global data
    global mark

    def recv():
        global data
        while True:
            data,addr = udp.recvfrom(1024)
            localtime = time.asctime(time.localtime(time.time()))
            data = str(localtime)+'\n'+data+'\n'
            msg.insert(tk.END,data)

    def sendMessage():
        global data
        global port
        port = udr[1]
        data = inp.get(0.0,tk.END)
        inp.delete(0.0,tk.END)
        data = '03#'+account+'#'+data+'#'
        main_send()

    def cle():
        inp.delete(0.0,tk.END)
        msg.delete(0.0,tk.END)

    def ref():
        global data
        global port
        global mark
        port = udr[1]
        data = '04#' + account + '#'
        mark = account
        main_send()

    def countTime():
        while True:
            ref()
            time.sleep(3)

    def close():
        chat.destroy()
        record.pop(account)

    # 检测是否打开了相同聊天对象窗口
    account = combo.get()

    if account in record:
        mb.showwarning('警告', '窗口已打开')
        return
    record[account] = count + 23678
    count += 1

    udp = sk.socket(sk.AF_INET, sk.SOCK_DGRAM)
    udr = ('localhost', 23678 + count - 1)
    udp.bind(udr)

    # 接收线程
    th = td.Thread(target=recv)
    th.setDaemon(True)
    th.start()

    # 刷新线程
    tc = td.Thread(target=countTime)
    tc.setDaemon(True)
    tc.start()

    # 聊天界面
    chat = tk.Tk()
    chat.title('I need u '+account)
    chat.iconbitmap('../icon/aowu.ico')
    width, height = 600, 450
    chat.geometry('%dx%d+%d+%d' % (
    width, height, (chat.winfo_screenwidth() - width) / 2, (chat.winfo_screenheight() - height) / 2))
    chat.maxsize(width=width, height=height)
    chat.minsize(width=width, height=height)
    msg = tk.Text(chat, height=21, width=82)
    msg.place(x=5, y=5)
    inp = tk.Text(chat, height=10, width=65)
    inp.place(x=5, y=296)
    c1 = ttk.Button(chat, text='发送',command=sendMessage)
    c1.place(x=487, y=300)
    c2 = ttk.Button(chat, text='清空',command=cle)
    c2.place(x=487, y=350)
    c3 = ttk.Button(chat, text='关闭',command=close)
    c3.place(x=487, y=400)

    # 检测是否为Unread
    if account == 'Unread':
        inp.place_forget()
        c1.place_forget()
        c2.place_forget()
        port = udr[1]
        data = '08#'
        main_send()

    chat.mainloop()

# socket
ADDR = ('202.114.196.97',21568)
data = 'result'
udpCliSock = sk.socket(sk.AF_INET, sk.SOCK_DGRAM)
udpCliSock.bind(('',6680))

# 主界面交互子线程
def main_recv():
    global data
    global port
    global record
    global mark
    global stop

    while True:
        data,addr = udpCliSock.recvfrom(1024)
        if stop:
            break
        if data=='03:01':
            data = '发送成功\n'
            udpCliSock.sendto(data,('localhost',port))

        elif data=='03:02':
            data = '对方已离线\n'
            udpCliSock.sendto(data,('localhost',port))

        elif data.startswith('08:'):
            data = data[3:]
            temp = record['Unread']
            udpCliSock.sendto('总消息数 '+data+'\n', ('localhost', temp))
            times = int(data)
            for i in range(times):
                udpCliSock.sendto('09#',ADDR)
                data,addr = udpCliSock.recvfrom(1024)
                data = data[3:]
                udpCliSock.sendto(data,('localhost',temp))

        elif data.startswith('04:'):
            if data != '04:0':
                data = data[3:]
                times = int(data)
                for i in range(times):
                    udpCliSock.sendto('05#'+mark+'#', ADDR)
                    data, addr = udpCliSock.recvfrom(1024)
                    data = data[3:]
                    udpCliSock.sendto(data, ('localhost', port))

def main_send():
    global data
    udpCliSock.sendto(data,ADDR)

# 登陆界面
height=144
width=360
root = tk.Tk()
root.title('I seek u')
root.iconbitmap('../icon/aowu.ico')
root.geometry('%dx%d+%d+%d' % (width, height, (root.winfo_screenwidth() - width ) / 2 , (root.winfo_screenheight() - height) / 2 ))
root.maxsize(360,144)
root.minsize(360,144)
entry1 = ttk.Combobox(root,width=18)
entry1.place(x=132, y=10)
number = open('../log/login.se','r')
data = number.read()
number.close()
data = data.split()
entry1['values']=tuple(data)
entry2 = ttk.Entry(root,width=20,show='*')
entry2.place(x=135, y=40)
entry3 = ttk.Entry(root,width=20,show='*')
entry3.place(x=135, y=70)
label1 = ttk.Label(root,width=7,text='输入账号')
label1.place(x=76,y=11)
label2 = ttk.Label(root,width=7,text='输入密码')
label2.place(x=76,y=41)
label3 = ttk.Label(root,width=7,text='确认密码')
label3.place(x=76,y=71)
button = ttk.Button(root, text='登陆/注册',width=28,command=sign)
button.place(x=76,y=98)
root.mainloop()

# 检测是否登陆成功
number = open('../log/check.se','r')
data = number.read()
number.close()

# 记录聊天对象账号
record = dict()

if data=='1':
    number = open('../log/check.se', 'w')
    number.write('0')
    number.close()

    # 用户界面
    user = tk.Tk()
    user.title('I miss u')
    user.iconbitmap('../icon/aowu.ico')
    user.geometry('%dx%d+%d+%d' % (width, height, (user.winfo_screenwidth() - width ) / 2 , (user.winfo_screenheight() - height) / 2 ))
    user.minsize(width,height)
    user.maxsize(width,height)
    label = ttk.Label(user,width=8,text='选择好友')
    label.place(x=72,y=18)
    ubutton_1 = ttk.Button(user,width=30,text='添加好友',command=inse)
    ubutton_1.place(x=72,y=51)
    ubutton_2 = ttk.Button(user,width=30,text='开始聊天',command=begin)
    ubutton_2.place(x=72,y=84)
    combo = ttk.Combobox(user,width=20)
    combo.place(x=126,y=17)
    combo['state']='readonly'
    number = open('../log/friends.se','r')
    data = number.read()
    number.close()
    data = data.split()
    combo['values']=tuple(data)
    th_recv = td.Thread(target=main_recv)
    th_recv.start()
    user.mainloop()

# 离线
stop = True
udpCliSock.sendto('06#',ADDR)
udpCliSock.close()