import operator


class LR:

    def __init__(self):
        self.input = list()
        self.productions = [['S\'', ['S']]]
        self.t = list()
        self.nt = list()
        self.items = list()
        self.map_table = list()
        self.paring_table = dict()
        self.string = ''
        self.file_stream = list()
        self.in_final = dict()

    def set_string(self, s):
        self.string = s.split(' ') + ['$']

    def add_input(self, input_string):
        self.input.append(input_string)

    def split(self):
        for production in self.input:
            left, rights = production.split('->')
            rights = rights.split('|')
            for right in rights:
                if right == 'e':
                    self.productions.append([left, []])
                else:
                    self.productions.append([left, right.split(' ')])

    def get_symbol(self):
        t, nt = list(), list()
        for production in self.productions:
            left = production[0]
            nt.append(left)
        for production in self.productions:
            rights = production[1]
            for right in rights:
                if right not in t:
                    t.append(right)
        self.t = list(set(t))
        self.nt = list(set(nt))
        for nt_name in self.nt:
            self.in_final[nt_name] = list()

    def generate_items(self):
        self.items.append([['S\'', ['S'], 0, ['$']]])
        item_index = 0
        while item_index < len(self.items):
            current_item = self.items[item_index]
            supply_index = 0
            look_token = dict()
            while supply_index < len(current_item):
                current_production = current_item[supply_index]
                point = current_production[2]
                if point < len(current_production[1]) and current_production[1][point] in self.nt:
                    target_token = current_production[1][point]
                    if target_token not in look_token:
                        look_token[target_token] = []
                    if point == len(current_production[1]) - 1:
                        look_token[target_token] += current_production[3]
                    elif current_production[1][point+1] in self.nt:
                        look_token[target_token] += self.in_final[current_production[1][point+1]]
                    else:
                        look_token[target_token] += [current_production[1][point+1]]
                    indexes = self.get_indexes(target_token)
                    for index in indexes:
                        target_production = self.productions[index]
                        new_productions = [target_production[0], target_production[1], 0, ['$']]
                        if LR.compare_products(current_item, new_productions):
                            current_item.append(new_productions)
                supply_index += 1
                for each_index, each_production in enumerate(current_item):
                    if each_production[0] in look_token:
                        current_item[each_index][3] = look_token[each_production[0]]
            self.simply_products(item_index)
            current_item = self.items[item_index]
            if not self.compare_item(current_item, item_index):
                self.items = self.items[:item_index] + self.items[item_index+1:]
                continue
            branch = dict()
            supply_index = 0
            while supply_index < len(current_item):
                current_production = current_item[supply_index]
                point = current_production[2]
                if point >= len(current_production[1]):
                    supply_index += 1
                    continue
                if current_production[1][point] not in branch:
                    branch[current_production[1][point]] = list()
                branch[current_production[1][point]].append([current_production[0], current_production[1], point+1,
                                                      current_production[3]])
                supply_index += 1
            for item in branch:
                if branch[item][0][2] <= len(branch[item][0][1]):
                    if self.compare_branch(branch[item]):
                        self.map_table.append([item_index, len(self.items), item])
                        self.items.append(branch[item])
                    else:
                        self.map_table.append([item_index, self.get_branch(branch[item]), item])
            item_index += 1

    def simply_products(self, item_index):
        new_item = list()
        current_item = self.items[item_index]
        for nm in current_item:
            nm[3] = list(set(nm[3]))
        for index in range(len(current_item)):
            current_production = current_item[index]
            mark = False
            for compare_index in range(index):
                compare_production = current_item[compare_index]
                if operator.eq(current_production, compare_production):
                    mark = True
            if not mark:
                new_item.append(current_production)
        self.items[item_index] = new_item

    def compare_item(self, item, item_index):
        for each_item in self.items[:item_index]:
            if len(each_item) == len(item):
                mark = True
                for index in range(len(item)):
                    if not operator.eq(item[index], each_item[index]):
                        mark = False
                if mark:
                    return False
        return True

    def compare_branch(self, item):
        min_length = len(item)
        for each_item in self.items:
            if len(each_item) >= min_length:
                mark = True
                for index in range(min_length):
                    if not operator.eq(each_item[index], item[index]):
                        mark = False
                if mark:
                    return False
        return True

    def get_branch(self, item):
        min_length = len(item)
        for each_index, each_item in enumerate(self.items):
            if len(each_item) >= min_length:
                mark = True
                for index in range(min_length):
                    if not operator.eq(each_item[index], item[index]):
                        mark = False
                if mark:
                    return each_index
        return -1

    def transform_parsing_table(self):
        for item in self.map_table:
            source = item[0]
            target = item[1]
            value = item[2]
            if source not in self.paring_table:
                self.paring_table[source] = dict()
            if value not in self.paring_table[source]:
                self.paring_table[source][value] = target
        out_state = set()
        in_state = set()
        for item in self.map_table:
            source = item[0]
            target = item[1]
            out_state.add(source)
            in_state.add(target)
        for item in self.map_table:
            target = item[1]
            if target in in_state and target not in out_state:
                if target not in self.paring_table:
                    self.paring_table[target] = dict()
                for value in self.items[target][0][3]:
                    self.paring_table[target][value] = [self.items[target][0][0], self.items[target][0][1]]
        for idx, item in enumerate(self.items):
            for pdx, production in enumerate(item):
                if pdx > 0:
                    for value in self.items[idx][pdx][3]:
                        self.paring_table[idx][value] = [self.items[idx][pdx][0], []]

    def analysis(self):
        tokens = self.string
        stack = ['$']
        state = [0]
        self.file_stream.append([str(state), str(stack), str(tokens)])
        while stack[-1] != 'S':
            if isinstance(self.paring_table[state[-1]][tokens[0]], int):
                stack.append(tokens[0])
                state.append(self.paring_table[state[-1]][tokens[0]])
                tokens = tokens[1:]
            else:
                stack_len = len(stack)
                state_len = len(state)
                token_len = len(self.paring_table[state[-1]][tokens[0]][1])
                add_new_token = self.paring_table[state[-1]][tokens[0]][0]
                stack = stack[:(stack_len-token_len)]
                state = state[:(state_len-token_len)]
                stack += [add_new_token]
                state += [self.paring_table[state[-1]][stack[-1]]]
            self.file_stream.append([str(state), str(stack), str(tokens)])

    @staticmethod
    def compare_products(source, target):
        for production in source:
            if operator.eq(production, target):
                return False
        return True

    def get_indexes(self, nt):
        indexes = list()
        for each_index, each_productions in enumerate(self.productions):
            if nt == each_productions[0]:
                indexes.append(each_index)
        return indexes

    def print(self):
        with open('table.txt', 'w') as f:
            f.write('State\tStack\tInput\n')
            for item in self.file_stream:
                f.write(str(item[0])+'\t'+str(item[1])+'\t'+str(item[2])+'\n')

    def transform(self):
        for key in self.in_final:
            self.in_final[key] = self.get_first(key)

    def get_first(self, target):
        ps = self.get_indexes(target)
        first_list = list()
        for index in ps:
            count = 0
            is_empty = True
            while is_empty and count < len(self.productions[index][1]):
                is_empty = False
                target = self.productions[index][1][count]
                if target in self.t:
                    first_list.append(target)
                else:
                    temp_first = self.get_first(target)
                    if 'e' in temp_first:
                        is_empty = True
                        count += 1
                    first_list += self.get_first(target)
        return list(set(first_list))


if __name__ == '__main__':
    lr = LR()
    lr.add_input('S->[ B')
    lr.add_input('A->int|[ B')
    lr.add_input('B->]|C')
    lr.add_input('C->A ]|A , C')
    lr.set_string('[ int , int ]')
    lr.split()
    lr.get_symbol()
    lr.transform()
    lr.generate_items()
    lr.transform_parsing_table()
    lr.analysis()
    lr.print()
