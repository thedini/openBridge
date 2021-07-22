#!/usr/bin/env python3
# a program to convert 29 bit extended frame headers in hex to pgns
# input needs to have stuffed bits removed
import sys

input = str(sys.argv[1])

num_of_bits = 29
scale = 16
# determine if SSR and IEP are included

def main():

#convert 21 bit hex to binary
  binary_input = bin(int(input, scale))[2:].zfill(num_of_bits)
  print("Identifier in Binary is: " + binary_input)

# select out the important bits
  priority = str(binary_input[0:3])
  priority = int(priority, 2)
  print("The Priority is: " + str(priority))
  reserve = str(binary_input[3])
  data_page = str(binary_input[4])
  pdu_field = str(binary_input[5:13])
  print("The PDU Field Format is: " + pdu_field + " and " + str(int(pdu_field, 2)))
  group_ext = str(binary_input[14:21])
  print("The Group Extension is: " + group_ext)

  pgn = "0"

# check if PDU Format Field is less than 240
  if int(pdu_field, 2) < 240:
      # calculate PGN
      pgn = reserve + data_page + pdu_field + "00000000"
  else:
      pgn = reserve + data_page + pdu_field + group_ext

# print PGN
  print("The PGN is: " + pgn + " and " + str(int(pgn, 2)) + " and " + str(hex(int(pgn, 2))))

if __name__ == "__main__":
    main()
