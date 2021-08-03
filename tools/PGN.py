#!/usr/bin/env python3

# a program to convert 29 bit extended frame headers in hex to pgns
# input needs to have stuffed bits removed
import sys
import json


def pgn_decrpt(found_pgn):
  file = open("PGN.json", 'r').read()
  file_data = json.loads(file)
  pgn_data = file_data["PGNs"]
  for pgn in pgn_data:
    if pgn["PGN"] == found_pgn:
      return pgn["Description"]
  return "ERROR :: No description was found for the PGN found"

def main():
  num_of_bits = 29
  scale = 16

# convert 21 bit hex to binary
  binary_input = bin(int(input, scale))[2:].zfill(num_of_bits)

# select out the important bits
  priority = str(binary_input[0:3])
  priority = int(priority, 2)
  reserve = str(binary_input[3])
  data_page = str(binary_input[4])
  pdu_field = str(binary_input[5:13])
  group_ext = str(binary_input[13:21])

# check if PDU Format Field is less than 240
  if int(pdu_field, 2) < 240:
    pgn = reserve + data_page + pdu_field + "00000000"
  else:
    pgn = reserve + data_page + pdu_field + group_ext

  print("Identifier in Binary is: " + binary_input)
  print("The Priority is: " + str(priority))
  print("The PDU Field Format is: " + pdu_field + " and " + str(int(pdu_field, 2)))
  print("The Group Extension is: " + group_ext)
  print("The PGN is: " + pgn + " and " + str(int(pgn, 2)) + " and " + str(hex(int(pgn, 2))))
  print(f"The PGN found above correlates to {pgn_decrpt(str(int(pgn, 2)))}")
if __name__ == "__main__":
  input = str(sys.argv[1])
  main()
