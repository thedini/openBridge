import requests
from bs4 import BeautifulSoup
import json

def main():
    json_data = {
        "PGNs": []
    }
    data = []
    for i in range(1,4):
        page = requests.get(f"https://www.nmea.org/content/STANDARDS/nmea_2000_pgn__search?txtPNGNumber=&optCategory=&txtPGNName=&rdSortBy=sNumber&whichpage={i}")
        soup = BeautifulSoup(page.text, 'html.parser')
        pgn = soup.find_all("b")
        for each in pgn:
            if any(map(str.isdigit, each.text)):
                data.append(f"{each.text}")
    # Gets rid of duplicate values
    data = list(set(data))

    for each in data:
            pgn_val = each[:each.index(" ")]
            decpt = each[each.index("- ")+2:]
            json_data["PGNs"].append({"PGN": pgn_val, "Description": decpt})
            print(pgn_val, decpt)

    with open("PGN.json", "w") as output: 
        json.dump(json_data, output, indent=4)

if __name__ == "__main__":
    main()