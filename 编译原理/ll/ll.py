class LL:

    def __init__(self):
        self.token = ''
        self.begin = ''
        self.exec_table = list()
        self.products = list()
        self.final = dict()
        self.in_final = dict()
        self.follow = dict()
        self.map_table = list()
        self.parsing_table = dict()

    def set_token(self, token):
        self.token = token

    def set_begin(self, begin):
        self.begin = begin

    def exec_token(self):
        stack = [self.begin, '$']
        input = self.token.split(' ') + ['$']
        action = self.parsing_table[stack[0]][input[0]]
        self.exec_table.append([str(stack), str(input), str(action)])
        while stack != ['$']:
            if stack[0] in self.in_final:
                action = self.parsing_table[stack[0]][input[0]]
                if action == ['e']:
                    stack = stack[1:]
                else:
                    stack = self.parsing_table[stack[0]][input[0]] + stack[1:]
            else:
                stack = stack[1:]
                input = input[1:]
                action = 'Terminal'
            self.exec_table.append([stack, input, action])

    def add_product(self, product):
        self.products.append(product)

    def split(self):
        temp_product = list()
        temp_final = set()
        count_final = 0
        for product in self.products:
            sp = product.split('->')
            left = sp[0]
            self.in_final[left] = list()
            right = sp[1].split('|')
            temp_product.append(right)
            for item in right:
                self.map_table.append([left, item])
        for product in temp_product:
            for part in product:
                sp = part.split(' ')
                for item in sp:
                    if item not in self.in_final:
                        temp_final.add(item)
        for item in temp_final:
            self.final[item] = count_final
            count_final += 1

    def transform(self):
        for key in self.in_final:
            self.in_final[key] = self.get_first(key)

    def get_first(self, target):
        ps = self.get_index(target)
        first_list = list()
        for index in ps:
            count = 0
            is_empty = True
            while is_empty and count < len(self.map_table[index][1].split(' ')):
                is_empty = False
                target = self.map_table[index][1].split(' ')[count]
                if target in self.final:
                    first_list.append(target)
                else:
                    temp_first = self.get_first(target)
                    if 'e' in temp_first:
                        is_empty = True
                        count += 1
                    first_list += self.get_first(target)
        return list(set(first_list))

    def get_sum(self):
        gs = 0
        for key in self.follow:
            gs += len(self.follow[key])
        return gs

    def transform_follow(self):
        for key in self.in_final.keys()[:1]:
            self.follow[key] = ['$']
        for key in self.in_final.keys()[1:]:
            self.follow[key] = []
        ngs = -1
        gs = 0
        while gs != ngs:
            gs = ngs
            for item in self.map_table:
                source = item[0]
                parts = item[1].split(' ')
                lp = len(parts)
                for part_index in range(lp):
                    begin = parts[part_index]
                    if begin in self.final:
                        continue
                    temp_index = part_index + 1
                    while temp_index < lp:
                        if parts[temp_index] in self.final:
                            if parts[temp_index] not in self.follow[begin]:
                                self.follow[begin].append(parts[temp_index])
                            break
                        else:
                            self.add_follow_by_first(begin, parts[temp_index])
                            if 'e' not in self.in_final[parts[temp_index]]:
                                break
                            else:
                                temp_index += 1
                    if temp_index == lp:
                        self.add_follow_by_follow(begin, source)
            ngs = self.get_sum()

    def add_follow_by_first(self, begin, end):
        for item in self.in_final[end]:
            if item != 'e':
                if item not in self.follow[begin]:
                    self.follow[begin].append(item)

    def add_follow_by_follow(self, begin, end):
        for item in self.follow[end]:
            if item != 'e':
                if item not in self.follow[begin]:
                    self.follow[begin].append(item)

    def get_index(self, value):
        indexes = list()
        for index in range(len(self.map_table)):
            if value == self.map_table[index][0]:
                indexes.append(index)
        return indexes

    def transform_parsing(self):
        for key in self.in_final:
            self.parsing_table[key] = dict()
        for item in self.map_table:
            begin = item[0]
            end = item[1].split(' ')[0]
            if end in self.in_final:
                for first in self.in_final[end]:
                    if first != 'e':
                        self.parsing_table[begin][first] = item[1].split(' ')
            else:
                if end != 'e':
                    self.parsing_table[begin][end] = item[1].split(' ')
            if 'e' in self.in_final[begin]:
                for follow in self.follow[begin]:
                    self.parsing_table[begin][follow] = ['e']

    def print(self):
        with open('table.txt', 'w') as f:
            f.write('Stack\tInput\tAction\n')
            for item in self.exec_table:
                f.write(str(item[0])+'\t'+str(item[1])+'\t'+str(item[2])+'\n')


if __name__ == '__main__':
    ll = LL()
    ll.set_begin('E')
    ll.add_product('E->T X')
    ll.add_product('X->+ E|e')
    ll.add_product('T->( E )|int Y')
    ll.add_product('Y->* T|e')
    token = 'int * int'
    ll.set_token(token)
    ll.split()
    ll.transform()
    ll.transform_follow()
    ll.transform_parsing()
    ll.exec_token()
    ll.print()

