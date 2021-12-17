# steganography
import cv2
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
from math import ceil
from codec import Codec, CaesarCypher, HuffmanCodes


class Steganography():

    def __init__(self):
        self.text = ''
        self.binary = ''
        self.delimiter = '#'
        self.codec = None

    def encode(self, filein, fileout, message, codec):
        image = cv2.imread(filein)
        print(image)  # for debugging

        # calculate available bytes
        max_bytes = image.shape[0] * image.shape[1] * 3 // 8
        print("Maximum bytes available:", max_bytes)

        # convert into binary
        if codec == 'binary':
            self.codec = Codec()
        elif codec == 'caesar':
            self.codec = CaesarCypher()
        elif codec == 'huffman':
            self.codec = HuffmanCodes()
        if self.codec != 'huffman':
            binary = self.codec.encode(message)
        else:
            binary = self.codec.encode(message)


        # check if possible to encode the message
        num_bytes = ceil(len(binary) // 8) + 1
        if num_bytes > max_bytes:
            print("Error: Insufficient bytes!")
        else:
            print("Bytes to encode:", num_bytes)
            self.text = message + self.delimiter
            self.binary = binary
            count = 0
            for i in np.nditer(image, op_flags=['readwrite']):
                if count < len(self.binary):
                    if i % 2 == 0 and self.binary[count] == '1':
                        i[...] = i + 1
                    if i % 2 == 0 and self.binary[count] == '0':
                        i[...] = i
                    if i % 2 == 1 and self.binary[count] == '0':
                        i[...] = i - 1
                    if i % 2 == 1 and self.binary[count] == '1':
                        i[...] = i
                count += 1
            print(image)
            cv2.imwrite(fileout+'.png',image)

    def decode(self, filein, codec):
        image = cv2.imread(filein)
        print(image)  # for debugging
        # convert into text
        if codec == 'binary':
            self.codec = Codec()
            data = ''
            z = ''
            for i in np.nditer(image,flags=['refs_ok']):
                if i % 2 == 1:
                    data += '1'
                else:
                    data += '0'
            self.text = self.codec.decode(data)
            self.binary = self.codec.encode(self.text)
            return self.text
        elif codec == 'caesar':
            self.codec = CaesarCypher()
            data = ''
            for i in np.nditer(image, flags=['refs_ok']):
                if i % 2 == 1:
                    data += '1'
                else:
                    data += '0'
            self.text = self.codec.decode(data)
            self.binary = self.codec.encode(self.text)
            return self.text
        elif codec == 'huffman':
            flag = True
            if self.codec == None or self.codec.name != 'huffman':
                print("A Huffman tree is not set!")
                flag = False
            if flag:
                data = ''
                for i in np.nditer(image, flags=['refs_ok']):
                    if i % 2 == 1:
                        data += '1'
                    else:
                        data += '0'
                self.text = self.codec.decode(data)
                self.binary = self.codec.encode(self.text)
    def print(self):
        if self.text == '':
            print("The message is not set.")
        else:
            print("Text message:", self.text)
            print("Binary message:", self.binary)
    def show(self, filename):
        plt.imshow(mpimg.imread(filename))
        plt.show()
