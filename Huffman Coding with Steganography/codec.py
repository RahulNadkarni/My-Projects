# codecs
import numpy as np
import steganography as s
import pickle
class Codec():

    def __init__(self):
        self.name = 'binary'
        self.delimiter = '00100011'  # a hash symbol '#'

    # convert text or numbers into binary form
    def encode(self, text):
        if type(text) == str:
            text += '#'
            return ''.join([format(ord(i), "08b") for i in text])
        else:
            print('Format error')

    # convert binary data into text
    def decode(self, data):
        binary = []
        x = ''
        for i in range(0, len(data), 8):
            byte = data[i: i + 8]
            if byte == self.delimiter:
                break
            x += byte
            binary.append(byte)
        text = ''
        for byte in binary:
            text += chr(int(byte, 2))
        return text

class CaesarCypher(Codec):
    def __init__(self, shift=3):
        super().__init__()
        self.name = 'caesar'
        self.delimiter = '00100011'  # you may need to set up it to a corresponding binary code
        self.shift = shift
        self.chars = 256  # total number of characters

    # convert text into binary form
    # your code should be similar to the corresponding code used for Codec
    def encode(self, text):
        data = ''
        # your code goes here
        if type(text) == str:
            text += '#'
            data = data.join([format(ord(i) + self.shift, "08b") for i in text])
            return data
        else:
            print("format error")

    # convert binary data into text
    # your code should be similar to the corresponding code used for Codec
    def decode(self, data):
        text = ''
        binary = []
        for i in range(0, len(data), 8):
            byte = data[i: i + 8]
            if byte == self.delimiter:
                break
            binary.append(byte)
        text = ''
        for byte in binary:
            text += chr(int(byte, 2))
        decipher = ''
        for char in text:
            if chr(ord(char) - self.shift) != '#':
                decipher += chr(ord(char) - self.shift)
            elif chr(ord(char) - self.shift) == '#':
                break
        return decipher
    # a helper class used for class HuffmanCodes that implements a Huffman tree
class Node:
    def __init__(self, freq, symbol, left=None, right=None):
        self.left = left
        self.right = right
        self.freq = freq
        self.symbol = symbol
        self.code = ''


class Stack:
    def __init__(self):
        self.items = []

    def isEmpty(self):
        return self.items == []

    def push(self, item):
        self.items.append(item)

    def pop(self):
        return self.items.pop()

    def peek(self):
        return self.items[len(self.items) - 1]

    def size(self):
        return len(self.items)
class HuffmanCodes(Codec):

    def __init__(self):
        super().__init__()
        self.nodes = None
        self.name = 'huffman'
        self.stack = Stack()
        self.s = {}
        self.saved = ()
        self.e = {}
    # make a Huffman Tree
    def make_tree(self, data):
        # make nodes
        nodes = []
        for char, freq in data.items():
            nodes.append(Node(freq, char))

        # assemble the nodes into a tree
        while len(nodes) > 1:
            # sort the current nodes by frequency
            nodes = sorted(nodes, key=lambda x: x.freq)

            # pick two nodes with the lowest frequencies
            left = nodes[0]
            right = nodes[1]

            # assign codes
            left.code = '0'
            right.code = '1'

            # combine the nodes into a tree
            root = Node(left.freq + right.freq, left.symbol + right.symbol,
                        left, right)

            # remove the two nodes and add their parent to the list of nodes
            nodes.remove(left)
            nodes.remove(right)
            nodes.append(root)
        self.nodes = nodes
        return nodes

    # traverse a Huffman tree
    def traverse_tree(self, node, val):
        x = ''
        next_val = val + node.code
        if (node.left):
            self.traverse_tree(node.left, next_val)
        if (node.right):
            self.traverse_tree(node.right, next_val)
        if (not node.left and not node.right):
            self.s[node.symbol] = next_val
    def decodetree(self,node,val):
        x = ''
        next_val = val + node.code
        if (node.left):
            self.decodetree(node.left, next_val)
        if (node.right):
            self.decodetree(node.right, next_val)
        if (not node.left and not node.right):
            self.e[next_val] = node.symbol
    # convert text into binary form
    def encode(self, text):
        text += '#'
        data = ''
        x = ''
        a = self.maked(text)
        (self.traverse_tree(self.make_tree(a)[0],data))
        for i in range(len(text)):
            if text[i] in self.s:
                x += self.s[text[i]]

        return x
    # convert binary data into text
    def maked(self,text):
        for c in text.lower():
            if c not in self.f:
                self.f[c] = 0
            self.f[c] += 1
        return self.f
    def decode(self,data):
        full = ''
        val = ''
        text = ''
        self.decodetree(self.nodes[0],val)
        for i in range(len(data)):
            full += data[i]
            z = ''
            if full in self.e:
                if self.e[full] == '#':
                    break
                text += self.e[full]
                z += full
                full = ''
        return text


# driver program for codec classes
if __name__ == '__main__':
    text = 'hello'
    print('Original:', text)

    c = Codec()
    binary = c.encode(text)
    print('Binary:', binary)
    data = c.decode(binary)
    print('Text:', data)

    cc = CaesarCypher()
    binary = cc.encode(text)
    print('Binary:', binary)
    data = cc.decode(binary)
    print('Text:', data)

    h = HuffmanCodes()
    binary = h.encode(text)
    print('Binary:', binary)
    data = h.decode(binary)
    print('Text:', data)
