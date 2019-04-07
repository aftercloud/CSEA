import copy as cp


class Item:

    def __init__(self):
        self.begin_index = 0
        self.size = 0
        self.item_string = ''
        self.net = list()
        self.final = [0]

    def set_item_string(self, item_string):
        self.item_string = item_string
        self.size = len(item_string)

    def set_begin_index(self, begin_index):
        self.begin_index = begin_index

    def get_next_index(self):
        return self.get_depth()

    def get_symbol(self, index):
        return self.item_string[index]

    def get_size(self):
        return self.size

    def get_depth(self):
        return self.net[-1][1]

    def remark(self):
        mark = self.begin_index
        for index in range(len(self.net)):
            if self.net[index][0] > 0:
                self.net[index][0] += mark
            self.net[index][1] += mark

    def all_remark(self):
        mark = 1
        for index in range(len(self.net)):
            self.net[index][0] += mark
            self.net[index][1] += mark
        for index in range(len(self.final)):
            self.final[index] += mark

    def all_remark_index(self, index):
        mark = index
        new_net = cp.deepcopy(self.net)
        for index in range(len(new_net)):
            new_net[index][0] += mark
            new_net[index][1] += mark
        return new_net

    def all_final_index(self, length):
        mark = length
        new_net = cp.deepcopy(self.final)
        for index in range(len(new_net)):
            new_net[index] += mark
        return new_net

    def transform(self):
        for index in range(self.get_size()):
            self.net.append([index, index+1, self.get_symbol(index), True])

    @staticmethod
    def merge(item1, item2):
        new_final = []
        count = 0
        last = item1.net[item1.final[0]][1]
        for ind in item1.final:
            i = ind + count
            insert_value = item1.net[i][1]
            new_net = item2.all_remark_index(insert_value)
            length = i + 1
            new_final += item2.all_final_index(length)
            item1.net[i][3] = False
            item1.net[i][1] = last
            for item in new_net:
                item1.net.insert(length, item)
                length += 1
            count += len(new_net)
            last = new_net[-1][1] + 1
        item1.final = new_final
        return item1

    def circle(self):
        self.all_remark()
        self.net.insert(0, [0, 1, 'e', False])
        new_final = list()
        for j in range(len(self.final)):
            i = self.final[j]
            insert_index = i + j * 2 + 1
            self.net[insert_index-1][3] = False
            insert_value = self.net[insert_index-1][1] + j
            self.net[insert_index-1][1] = insert_value
            self.net.insert(insert_index, [insert_value, 1, 'e', False])
            self.net.insert(insert_index+1, [1, insert_value+1, 'e', True])
            new_final.append(insert_index+1)
        self.final = new_final

    @staticmethod
    def addtion(item1, item2):
        new_item = Item()
        item2.set_begin_index(item1.get_next_index())
        item2.remark()
        length = len(item1.net)
        for index in range(len(item2.final)):
            item2.final[index] += length
        new_item.net = item1.net + item2.net
        new_item.final = item1.final + item2.final
        return new_item

    def print(self):
        for it in self.net:
            print(it)


class Items:

    def __init__(self):
        self.items = list()
        self.origin_string = ''
        self.middle_string = ''

    def set_origin_string(self, origin):
        self.origin_string = origin

    def get_origin_string(self):
        return self.origin_string

    def get_length(self):
        return len(self.origin_string)

    def get_size(self):
        return len(self.items)

    def get_symbol(self, index):
        return self.origin_string[index]

    def get_symbols(self, index_begin, index_end):
        return self.origin_string[index_begin:index_end]

    def get_last_index(self):
        if self.get_size() > 0:
            return self.items[-1].get_next_index()
        return 0

    def generate_item(self, item_string):
        item = Item()
        item.set_item_string(item_string)
        item.transform()
        item.remark()
        self.items.append(item)

    @staticmethod
    def get_code_set(item):
        origin = item.get_origin_string()
        code_set = ''
        for char in origin:
            if char not in ['(', ')', '+', '*', 'e']:
                if char not in code_set:
                    code_set += char
        return code_set

    @staticmethod
    def is_char_valid(char):
        return char not in ['(', ')', '+', '*', 'e']

    def transform_middle_mode(self):
        index_begin = 0
        length = self.get_length()
        while index_begin < length:
            if Items.is_char_valid(self.get_symbol(index_begin)):
                self.generate_item(self.get_symbol(index_begin))
                self.middle_string += 'e'
            else:
                self.middle_string += self.get_symbol(index_begin)
            index_begin += 1

    def transform_v2(self):
        while self.middle_string != 'e':
            if self.middle_string.count('e*') > 0:
                e_count = -1
                new_middle_string = ''
                for char_index in range(len(self.middle_string)):
                    if self.middle_string[char_index] == 'e':
                        e_count += 1
                        if char_index+1 < len(self.middle_string) and self.middle_string[char_index+1] == '*':
                            self.items[e_count].circle()
                    elif self.middle_string[char_index] == '*' and self.middle_string[char_index-1] == 'e':
                            continue
                    new_middle_string += self.middle_string[char_index]
                self.middle_string = new_middle_string
            while self.middle_string.count('ee') > 0:
                e_count = -1
                new_middle_string = ''
                new_items = list()
                length = len(self.middle_string)
                char_index = 0
                while char_index < length:
                    if self.middle_string[char_index] == 'e':
                        e_count += 1
                        if char_index+1 < len(self.middle_string) and self.middle_string[char_index+1] == 'e':
                            new_items.append(Item.merge(self.items[e_count], self.items[e_count+1]))
                            e_count += 1
                            char_index += 1
                        else:
                            new_items.append(self.items[e_count])
                    new_middle_string += self.middle_string[char_index]
                    char_index += 1
                self.middle_string = new_middle_string
                self.items = new_items
            while self.middle_string.count('(e)') > 0:
                self.middle_string = self.middle_string.replace('(e)', 'e')
            if 'ee' not in self.middle_string and 'e*' not in self.middle_string and '(e)' not in self.middle_string:
                while self.middle_string.count('e+e') > 0:
                    e_count = -1
                    new_middle_string = ''
                    new_items = list()
                    length = len(self.middle_string)
                    char_index = 0
                    while char_index < length:
                        if self.middle_string[char_index] == 'e':
                            e_count += 1
                            if char_index+1 < len(self.middle_string) and self.middle_string[char_index+1] == '+':
                                new_items.append(Item.addtion(self.items[e_count], self.items[e_count+1]))
                                e_count += 1
                                char_index += 2
                            else:
                                new_items.append(self.items[e_count])
                        new_middle_string += self.middle_string[char_index]
                        char_index += 1
                    self.middle_string = new_middle_string
                    self.items = new_items

    def get_item_net(self):
        return self.items[0]

    def print(self):
        for it in self.items:
            it.print()
            print('')


class ItemV2:

    def __init__(self):
        self.net = list()
        self.set = list()
        self.table = list()
        self.map_table = list()
        self.final = list()
        self.code_set = ''

    def set_code_set(self, code_set):
        self.code_set = code_set

    def set_net(self, net):
        self.net = net.net

    def get_index_by_begin(self, begin, value):
        size = len(self.net)
        result = list()
        for i in range(size):
            if begin == self.net[i][0] and value == self.net[i][2]:
                result.append(self.net[i][1])
        return result

    def transform(self):
        table_index = 0
        temp_list = [[0]]
        for temp_index in range(len(self.code_set)):
            temp_list.append([])
        self.table.append(temp_list)
        self.set.append(str([0]))
        table_size = len(self.table)
        while table_index < table_size:
            for begin in self.table[table_index][0]:
                for symbol_index in range(len(self.code_set)):
                    symbol = self.code_set[symbol_index]
                    tb = begin
                    begin_item = self.get_index_by_begin(tb, symbol)
                    begin_index = 0
                    while begin_index < len(begin_item):
                        begin_item += self.get_index_by_begin(begin_item[begin_index], symbol)
                        begin_index += 1
                    begin_index = 0
                    while begin_index < len(begin_item):
                        begin_item += self.get_index_by_begin(begin_item[begin_index], 'e')
                        begin_index += 1
                    begin_item.sort()
                    self.table[table_index][symbol_index+1] += begin_item
            for symbol_index in range(len(self.code_set)):
                if str(self.table[table_index][symbol_index+1]) not in self.set:
                    self.set.append(str(self.table[table_index][symbol_index+1]))
                    temp_list = [self.table[table_index][symbol_index+1]]
                    for temp_index in range(len(self.code_set)):
                        temp_list.append([])
                    self.table.append(temp_list)
            table_index += 1
            table_size = len(self.table)

    def get_final_state(self):
        finals = []
        for item in self.net:
            if item[3]:
                finals.append(item[1])
        return finals

    def get_final_num(self):
        finals = self.get_final_state()
        nums = list()
        for index in range(1, len(self.table)):
            state_list = self.table[index][0]
            for fm in finals:
                if fm in state_list:
                    nums.append(index)
                    break
        return nums

    def transfiorm_v2(self):
        st = list()
        for ts in self.table:
            tl = list()
            for ls in ts:
                tl.append(str(ls))
            st.append(tl)
        for i in range(len(self.table)):
            for j in range(len(self.table[i])-1):
                target = st[i][j+1]
                target_index = 0
                for k in range(len(st)):
                    if st[k][0] == target:
                        target_index = k
                        break
                self.map_table.append([i, target_index, self.code_set[j]])
        middle_table = self.map_table[:]
        final_nums = self.get_final_num()
        in_final_nums = []
        for index in range(len(self.table)):
            if index not in final_nums:
                in_final_nums.append(index)
        self.map_table = []
        final_table = []
        in_final_table = []
        for index in final_nums:
            final_table.append(self.table[index])
        for index in in_final_nums:
            in_final_table.append(self.table[index])
        dict_set = dict()
        for index in range(len(final_table)):
            mark = ''
            for num_index in range(len(self.code_set)):
                mark += str(final_table[index][num_index+1])
            if mark in dict_set:
                dict_set[mark].append(index)
            else:
                dict_set[mark] = [index]
        result_final_list = list()
        for key in dict_set:
            result_final_list.append(dict_set[key])
        dict_set = dict()
        for index in range(len(in_final_table)):
            mark = ''
            for num_index in range(len(self.code_set)):
                mark += str(in_final_table[index][num_index+1])
            if mark in dict_set:
                dict_set[mark].append(index)
            else:
                dict_set[mark] = [index]
        result_in_final_list = list()
        for key in dict_set:
            result_in_final_list.append(dict_set[key])
        for ii in range(len(result_final_list)):
            for ij in range(len(result_final_list[ii])):
                result_final_list[ii][ij] = final_nums[result_final_list[ii][ij]]
        for ii in range(len(result_in_final_list)):
            for ij in range(len(result_in_final_list[ii])):
                result_in_final_list[ii][ij] = in_final_nums[result_in_final_list[ii][ij]]
        for item in result_final_list:
            ft = item[0]
            for it in item:
                for ti in range(len(middle_table)):
                    for tj in range(len(middle_table[ti])):
                        if middle_table[ti][tj] == it:
                            middle_table[ti][tj] = ft
        for item in result_in_final_list:
            ft = item[0]
            for it in item:
                for ti in range(len(middle_table)):
                    for tj in range(len(middle_table[ti])):
                        if middle_table[ti][tj] == it:
                            middle_table[ti][tj] = ft
        dict_set = dict()
        mark_list = list()
        for ind in middle_table:
            si = str(ind)
            if si in dict_set:
                mark_list.append(False)
            else:
                dict_set[si] = True
                mark_list.append(True)
        map_list = list()
        for num in range(len(middle_table)):
            if mark_list[num]:
                map_list.append(middle_table[num])
        self.map_table = map_list
        self.final = result_final_list

    def get_map_table(self):
        return self.map_table

    def get_final(self):
        first_final = list()
        for item in self.final:
            first_final.append(item[0])
        return first_final


class Remark:

    def __init__(self):
        self.map_table = list()
        self.code_set = set()
        self.code_list = list()
        self.final = list()
        self.map = dict()

    def set_final(self, final):
        self.final = final

    def set_map_table(self, map_table):
        self.map_table = map_table

    def exec_code_set(self):
        for i in range(len(self.map_table)):
            for j in range(len(self.map_table[i])-1):
                self.code_set.add(self.map_table[i][j])
        self.code_list = list(self.code_set)

    def print(self):
        with open('table.txt', 'w') as f:
            for i in range(len(self.map_table)):
                for j in range(len(self.map_table[i])):
                    f.write(str(self.map_table[i][j])+'\t')
                f.write('\n')
            for final in self.final:
                f.write(str(self.map[final])+'\t')

    def exec_remark(self):
        for k in range(len(self.code_list)):
            change = self.code_list[k]
            self.map[change] = k
            for i in range(len(self.map_table)):
                for j in range(len(self.map_table[i])-1):
                    if self.map_table[i][j] == change:
                        self.map_table[i][j] = k


if __name__ == '__main__':
    origin_string = input('特殊字符 ()+*e\n正则表达式 = ')
    items = Items()
    items.set_origin_string(origin_string)
    items.transform_middle_mode()
    items.transform_v2()
    iv2 = ItemV2()
    iv2.set_net(items.get_item_net())
    iv2.set_code_set(Items.get_code_set(items))
    iv2.transform()
    iv2.transfiorm_v2()
    remark = Remark()
    remark.set_map_table(iv2.get_map_table())
    remark.exec_code_set()
    remark.set_final(iv2.get_final())
    remark.exec_remark()
    remark.print()

