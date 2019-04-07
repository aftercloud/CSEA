import pyforms
from pyforms import BaseWidget
from pyforms.controls import *


class Banker(BaseWidget):

    def __init__(self):
        super(Banker, self).__init__("银行家模拟器")
        self._label_1 = ControlLabel('客户')
        self._label_2 = ControlLabel('现金')
        self._label_3 = ControlLabel('黄金')
        self._label_4 = ControlLabel('石油')
        self._label_5 = ControlLabel('钢铁')
        self._text_1 = ControlLabel('鱼/4/6/6/5')
        self._text_1_1 = ControlLabel('0')
        self._text_1_2 = ControlLabel('0')
        self._text_1_3 = ControlLabel('0')
        self._text_1_4 = ControlLabel('0')
        self._text_2 = ControlLabel('好/3/6/2/5')
        self._text_2_1 = ControlLabel('0')
        self._text_2_2 = ControlLabel('0')
        self._text_2_3 = ControlLabel('0')
        self._text_2_4 = ControlLabel('0')
        self._text_3 = ControlLabel('大/2/3/8/9')
        self._text_3_1 = ControlLabel('0')
        self._text_3_2 = ControlLabel('0')
        self._text_3_3 = ControlLabel('0')
        self._text_3_4 = ControlLabel('0')
        self._text_4 = ControlLabel('的/4/9/9/10')
        self._text_4_1 = ControlLabel('0')
        self._text_4_2 = ControlLabel('0')
        self._text_4_3 = ControlLabel('0')
        self._text_4_4 = ControlLabel('0')
        self._input_1 = ControlText()
        self._input_2 = ControlText()
        self._input_3 = ControlText()
        self._input_4 = ControlText()
        self._input_5 = ControlText()
        self._input_6 = ControlLabel('剩余')
        self._input_7 = ControlLabel('6')
        self._input_8 = ControlLabel('10')
        self._input_9 = ControlLabel('10')
        self._input_10 = ControlLabel('15')
        self._input_11 = ControlLabel('总量')
        self._input_12 = ControlLabel('6')
        self._input_13 = ControlLabel('10')
        self._input_14 = ControlLabel('10')
        self._input_15 = ControlLabel('15')
        self._btn_1 = ControlButton('申请')
        self._btn_2 = ControlButton('重置')
        self._textArea = ControlTextArea('安全队列')
        self.formset = [{
            '申请': [('_label_1', '_label_2', '_label_3', '_label_4', '_label_5'),
                   ('_text_1', '_text_1_1', '_text_1_2', '_text_1_3', '_text_1_4'),
                   ('_text_2', '_text_2_1', '_text_2_2', '_text_2_3', '_text_2_4'),
                   ('_text_3', '_text_3_1', '_text_3_2', '_text_3_3', '_text_3_4'),
                   ('_text_4', '_text_4_1', '_text_4_2', '_text_4_3', '_text_4_4'),
                   ('_input_6', '_input_7', '_input_8', '_input_9', '_input_10'),
                   ('_input_11', '_input_12', '_input_13', '_input_14', '_input_15'),
                   ('_input_1', '_input_2', '_input_3', '_input_4', '_input_5'),
                   ('_btn_1', '_btn_2')],
            '查询': ['_textArea']
        }]
        self._btn_1.value = self.toDo
        self._btn_2.value = self.reSet
        # 记录最大值和当前值
        self.data0_1 = [6, 10, 10 ,15]
        self.data0_2 = [6, 10, 10, 15]
        self.data1_1 = [4, 6, 6, 5]
        self.data1_2 = [0, 0, 0, 0]
        self.data2_1 = [3, 6, 2, 5]
        self.data2_2 = [0, 0, 0, 0]
        self.data3_1 = [2, 3, 8, 9]
        self.data3_2 = [0, 0, 0, 0]
        self.data4_1 = [4, 9, 9, 10]
        self.data4_2 = [0, 0, 0, 0]

        self.mark = True

    def toDo(self):
        client = self._input_1.value
        data = [int(self._input_2.value), int(self._input_3.value), int(self._input_4.value), int(self._input_5.value)]
        data1 = self.data1_2[:]
        data2 = self.data2_2[:]
        data3 = self.data3_2[:]
        data4 = self.data4_2[:]
        record = ''
        toUse = self.data0_2[:]
        for i in range(4):
            toUse[i] -= data[i]
        if client == '鱼':
            for i in range(4):
                data1[i] += data[i]
        elif client == '好':
            for i in range(4):
                data2[i] += data[i]
        elif client == '大':
            for i in range(4):
                data3[i] += data[i]
        else:
            for i in range(4):
                data4[i] += data[i]
        nm = [0, 0, 0, 0]
        nm[0] = sum(self.data1_1) - sum(data1)
        nm[1] = sum(self.data2_1) - sum(data2)
        nm[2] = sum(self.data3_1) - sum(data3)
        nm[3] = sum(self.data4_1) - sum(data4)
        for i in range(4):
            MIN = 10000
            INDEX = 0
            for j in range(4):
                if MIN > nm[j]:
                    MIN = nm[j]
                    INDEX = j
            nm[INDEX] = 10000
            if INDEX == 0:
                for i in range(4):
                    if data1[i] + toUse[i] < self.data1_1[i]:
                        self.mark = False
                        break
                    toUse[i] += data1[i]
                record += '鱼 -> '
            elif INDEX == 1:
                for i in range(4):
                    if data2[i] + toUse[i] < self.data2_1[i]:
                        self.mark = False
                        break
                    toUse[i] += data2[i]
                record += '好 -> '
            elif INDEX == 2:
                for i in range(4):
                    if data3[i] + toUse[i] < self.data3_1[i]:
                        self.mark = False
                        break
                    toUse[i] += data3[i]
                record += '大 -> '
            else:
                for i in range(4):
                    if data4[i] + toUse[i] < self.data4_1[i]:
                        self.mark = False
                        break
                    toUse[i] += data4[i]
                record += '的 -> '
        if not self.mark:
            self.mark = True
            record += 'ERROR\n'
            self._textArea.value += record
        else:
            for i in range(4):
                self.data0_2[i] -= data[i]
            self._input_7.value = str(self.data0_2[0])
            self._input_8.value = str(self.data0_2[1])
            self._input_9.value = str(self.data0_2[2])
            self._input_10.value = str(self.data0_2[3])
            if client == '鱼':
                for i in range(4):
                    self.data1_2[i] = data1[i]
                self._text_1_1.value = str(data1[0])
                self._text_1_2.value = str(data1[1])
                self._text_1_3.value = str(data1[2])
                self._text_1_4.value = str(data1[3])
            elif client == '好':
                for i in range(4):
                    self.data2_2[i] = data2[i]
                self._text_2_1.value = str(data2[0])
                self._text_2_2.value = str(data2[1])
                self._text_2_3.value = str(data2[2])
                self._text_2_4.value = str(data2[3])
            elif client == '大':
                for i in range(4):
                    self.data3_2[i] = data3[i]
                self._text_3_1.value = str(data3[0])
                self._text_3_2.value = str(data3[1])
                self._text_3_3.value = str(data3[2])
                self._text_3_4.value = str(data3[3])
            else:
                for i in range(4):
                    self.data4_2[i] = data4[i]
                self._text_4_1.value = str(data4[0])
                self._text_4_2.value = str(data4[1])
                self._text_4_3.value = str(data4[2])
                self._text_4_4.value = str(data4[3])
            record += 'OK\n'
            self._textArea.value += record

    def reSet(self):
        self.data0_1 = [6, 10, 10, 15]
        self.data0_2 = [6, 10, 10, 15]
        self.data1_1 = [4, 6, 6, 5]
        self.data1_2 = [0, 0, 0, 0]
        self.data2_1 = [3, 6, 2, 5]
        self.data2_2 = [0, 0, 0, 0]
        self.data3_1 = [2, 3, 8, 9]
        self.data3_2 = [0, 0, 0, 0]
        self.data4_1 = [4, 9, 9, 10]
        self.data4_2 = [0, 0, 0, 0]
        self._input_7.value = str(self.data0_2[0])
        self._input_8.value = str(self.data0_2[1])
        self._input_9.value = str(self.data0_2[2])
        self._input_10.value = str(self.data0_2[3])
        self._text_1_1.value = str(self.data1_2[0])
        self._text_1_2.value = str(self.data1_2[1])
        self._text_1_3.value = str(self.data1_2[2])
        self._text_1_4.value = str(self.data1_2[3])
        self._text_2_1.value = str(self.data2_2[0])
        self._text_2_2.value = str(self.data2_2[1])
        self._text_2_3.value = str(self.data2_2[2])
        self._text_2_4.value = str(self.data2_2[3])
        self._text_3_1.value = str(self.data3_2[0])
        self._text_3_2.value = str(self.data3_2[1])
        self._text_3_3.value = str(self.data3_2[2])
        self._text_3_4.value = str(self.data3_2[3])
        self._text_4_1.value = str(self.data4_2[0])
        self._text_4_2.value = str(self.data4_2[1])
        self._text_4_3.value = str(self.data4_2[2])
        self._text_4_4.value = str(self.data4_2[3])
        self._textArea.value += '\n'


if __name__ == "__main__":
    pyforms.start_app(Banker)