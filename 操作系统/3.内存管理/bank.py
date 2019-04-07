import pyforms
from pyforms import BaseWidget
from pyforms.controls import *


class Memory(BaseWidget):

    def __init__(self):
        super(Memory, self).__init__("内存分配模拟器")
        # 进程字典
        # 键值对为编号和实例
        self.processes = dict()
        # GUI
        # 查询页
        self.textArea = ControlTextArea('内存使用情况')
        # 管理页
        # 第一行 内存地址
        self.memoryMark_0 = ControlLabel('0')
        self.memoryMark_1 = ControlLabel('1')
        self.memoryMark_2 = ControlLabel('2')
        self.memoryMark_3 = ControlLabel('3')
        self.memoryMark_4 = ControlLabel('4')
        self.memoryMark_5 = ControlLabel('5')
        self.memoryMark_6 = ControlLabel('6')
        self.memoryMark_7 = ControlLabel('7')
        self.memoryMark_8 = ControlLabel('8')
        self.memoryMark_9 = ControlLabel('9')
        self.memoryMark_10 = ControlLabel('10')
        self.memoryMark_11 = ControlLabel('11')
        self.memoryMark_12 = ControlLabel('12')
        self.memoryMark_13 = ControlLabel('13')
        self.memoryMark_14 = ControlLabel('14')
        self.memoryMark_15 = ControlLabel('15')
        # 第二行 内存使用进程标记
        self.memoryUser_0 = ControlLabel('')
        self.memoryUser_1 = ControlLabel('')
        self.memoryUser_2 = ControlLabel('')
        self.memoryUser_3 = ControlLabel('')
        self.memoryUser_4 = ControlLabel('')
        self.memoryUser_5 = ControlLabel('')
        self.memoryUser_6 = ControlLabel('')
        self.memoryUser_7 = ControlLabel('')
        self.memoryUser_8 = ControlLabel('')
        self.memoryUser_9 = ControlLabel('')
        self.memoryUser_10 = ControlLabel('')
        self.memoryUser_11 = ControlLabel('')
        self.memoryUser_12 = ControlLabel('')
        self.memoryUser_13 = ControlLabel('')
        self.memoryUser_14 = ControlLabel('')
        self.memoryUser_15 = ControlLabel('')
        self.memoryUser = [self.memoryUser_0, self.memoryUser_1, self.memoryUser_2, self.memoryUser_3,
                           self.memoryUser_4, self.memoryUser_5, self.memoryUser_6, self.memoryUser_7,
                           self.memoryUser_8, self.memoryUser_9, self.memoryUser_10, self.memoryUser_11,
                           self.memoryUser_12, self.memoryUser_13, self.memoryUser_14, self.memoryUser_15
                           ]
        self.memoryUserMark = [False] * 16
        for i in self.memoryUser:
            i.value = 'N'
        # 第三行 进程申请内存
        self.applyLabel = ControlLabel('进程申请')
        self.applyName = ControlText('进程名')
        self.applySpace = ControlText('申请内存大小')
        self.applyBtn = ControlButton('申请')
        # 第四行 进程撤销
        self.deleteLabel = ControlLabel('进程撤销')
        self.deleteName = ControlText('进程名')
        self.deleteBtn = ControlButton('撤销')
        # 第五行 算法选择
        self.algorithm = ControlCombo('算法选择')
        self.algorithm.add_item('首次适度算法', 0)
        self.algorithm.add_item('最优适度算法', 1)
        self.algorithm.add_item('邻近适度算法', 2)
        # 第六行 查询和重置
        self.query = ControlButton('查询')
        self.reset = ControlButton('重置')
        # 绑定按钮和事件
        self.applyBtn.value = self.apply
        self.deleteBtn.value = self.delete
        self.query.value = self.query_s
        self.reset.value = self.setMemoryEmpty
        # GUI布局
        self.formset = [{
            '管理': [(
                    'memoryMark_0', 'memoryMark_1', 'memoryMark_2', 'memoryMark_3',
                    'memoryMark_4', 'memoryMark_5', 'memoryMark_6', 'memoryMark_7',
                    'memoryMark_8', 'memoryMark_9', 'memoryMark_10', 'memoryMark_11',
                    'memoryMark_12', 'memoryMark_13', 'memoryMark_14', 'memoryMark_15'
                    ),
                   (
                       'memoryUser_0', 'memoryUser_1', 'memoryUser_2', 'memoryUser_3',
                       'memoryUser_4', 'memoryUser_5', 'memoryUser_6', 'memoryUser_7',
                       'memoryUser_8', 'memoryUser_9', 'memoryUser_10', 'memoryUser_11',
                       'memoryUser_12', 'memoryUser_13', 'memoryUser_14', 'memoryUser_15',
                   ),
                ('applyLabel', 'applyName', 'applySpace', 'applyBtn'),
                ('deleteLabel', 'deleteName', 'deleteBtn'),
                ('algorithm'),
                ('query', 'reset')
            ],
            '查询':['textArea']
        }]
        # 上次分配地址
        self.lastAddress = 0

    # 获取空闲内存页
    def getEmptyBlocks(self):
        blocks = list()
        i = 0
        while i < 16:
            if self.memoryUser[i].value == 'N':
                size = 1
                blocks.append(i)
                while i < 15:
                    i += 1
                    if self.memoryUser[i].value == 'N':
                        size += 1
                    else:
                        blocks.append(size)
                        break
            i += 1
        if len(blocks) & 1 == 1:
            temp = blocks[len(blocks)-1]
            blocks.append(16-temp)
        return blocks

    # 用进程名更新进程占用的内存页
    def changeMemory(self, name, index, size):
        for i in range(index, index+size):
            self.memoryUser[i].value = name

    # 撤销进程后重置对应内存页为空
    def resetMemory(self, index, size):
        for i in range(index, index+size):
            self.memoryUser[i].value = 'N'

    # 重置数据为空
    def setMemoryEmpty(self):
        for i in range(16):
            self.memoryUser[i].value = 'N'
        self.processes = dict()
        self.lastAddress = 0

    # 紧缩
    def tightening(self):
        for i in range(16):
            self.memoryUser[i].value = 'N'
        lastIndex = 0
        for i in self.processes:
            process = self.processes[i]
            process[0] = lastIndex
            self.changeMemory(i, process[0], process[1])
            self.lastAddress = lastIndex
            lastIndex += process[1]

    # 进程申请
    def apply(self):
        blocks = self.getEmptyBlocks()
        index = list()
        size = list()
        for i in range(int(len(blocks)/2)):
            index.append(blocks[i*2])
            size.append(blocks[i*2+1])
        need = int(self.applySpace.value)
        name = self.applyName.value
        # 若剩余空间不满足申请要求则紧缩
        if need > max(size):
            self.tightening()
        # 在紧缩的基础上若剩余空间不满足申请要求则输出失败信息
        blocks = self.getEmptyBlocks()
        index = list()
        size = list()
        for i in range(int(len(blocks)/2)):
            index.append(blocks[i*2])
            size.append(blocks[i*2+1])
        need = int(self.applySpace.value)
        name = self.applyName.value
        if need > max(size):
            s = '进程'
            s += name
            s += '申请失败\n'
            s += '当前最大空闲区域页数为'
            s += str(max(size))
            self.textArea.value = s
        # 若满足申请要求则根据选择的算法进行分配
        else:
            # 首次适度算法
            if self.algorithm.value == 0:
                for i in range(len(index)):
                    if need <= size[i]:
                        self.changeMemory(name, index[i], need)
                        self.processes[name] = [index[i], need]
                        self.lastAddress = index[i]
                        break
            # 最优适度算法
            elif self.algorithm.value == 1:
                bestSize = [i - need for i in size]
                size = []
                for i in bestSize:
                    if i >= 0:
                        size.append(i)
                bestIndex = 0
                bestMin = 16
                for i in range(len(size)):
                    if bestMin > size[i]:
                        bestMin = size[i]
                        bestIndex = i
                self.changeMemory(name, index[bestIndex], need)
                self.processes[name] = [index[bestIndex], need]
                self.lastAddress = index[bestIndex]
            # 邻近适度算法
            else:
                compare = self.lastAddress
                for i in range(len(index)):
                    if compare <= index[i] and need <= size[i]:
                        self.changeMemory(name, index[i], need)
                        self.processes[name] = [index[i], need]
                        self.lastAddress = index[i]
                        return
                for i in range(len(index)):
                    if need <= size[i]:
                        self.changeMemory(name, index[i], need)
                        self.processes[name] = [index[i], need]
                        self.lastAddress = index[i]

    # 进程撤销
    def delete(self):
        name = self.deleteName.value
        if name in self.processes:
            process = self.processes[name]
            del self.processes[name]
            self.resetMemory(process[0], process[1])

    # 查询
    def query_s(self):
        showValue = ''
        for i in self.processes:
            process = self.processes[i]
            s = '进程名：' + i
            s += '\t起始地址:'
            s += str(process[0])
            s += '\t进程大小:'
            s += str(process[1])
            s += '\n'
            showValue += s
        self.textArea.value = showValue


if __name__ == "__main__":
    pyforms.start_app(Memory)