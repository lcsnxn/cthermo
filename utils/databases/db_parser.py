"""
Transform the database files into more structured json files. 
"""

import json


def parse_chemsepdb() -> None:
    """
    Parse the `chemsepdb.pct` file into a more structured json file.
    The `chemsepdb.pct` is extracted from the ChemSep program database.
    """
    
    with open("chemsepdb.pct", "r") as file:
        data = file.read()
        
    components = data.split("[Component Text]")[1:]
    parsed_components = []

    for component in components:
        lines = component.strip().split("\n")
        component_dict = {}
        
        component_dict["Index"] = lines[0].split()[0].strip()
        component_dict["Name"] = lines[1].strip()
        component_dict["Formula"] = lines[2].strip()
        
        start_parsing = False
        
        for line in lines[3:]:
            line_elements = line.split()
            key = " ".join(line_elements[1:])
            value = line_elements[0]
            
            if "Ideal gas heat capacity" in key:
                start_parsing = True
                continue
            
            if start_parsing:
                component_dict[f"{key}_coeff"] = value
                if key == "D":
                    start_parsing = False
            
            if key.startswith("Family"):
                continue
            if (key.startswith("EqNo") and not "Ideal gas heat capacity" in key 
                or key.startswith("Min.") 
                or key.startswith("Max.")):
                continue
            if (key == "A" 
                or key == "B" 
                or key == "C" 
                or key == "D" 
                or key == "E"):
                continue
            if (key.endswith("UNIFAC") 
                or key.endswith("UNIFAC-LLE") 
                or key.endswith("ASOG") 
                or key.endswith("PPR78") 
                or key.endswith("UMR") 
                or key.endswith("Modified UNIFAC")):
                continue
            
            if key is not None and value is not None:
                component_dict[key] = value 
            
        if lines[-1][0].isdigit():
            component_dict["CASN"] = lines[-1]
        elif lines[-2][0].isdigit():
            component_dict["CASN"] = lines[-2]
        else:
            component_dict["CASN"] = None
            
        del component_dict[""]
                
        parsed_components.append(component_dict)

    with open("chemsepdb.json", "w") as file:
        json.dump(parsed_components, file, indent=2)
        
def build_names_db() -> None:
    """
    Build a database to store names and CASN numbers of elements.
    """
    
    with open("chemsepdb.json", "r") as file:
        db_file = json.load(file) 
    
    names_db = []
    for item in db_file:
        try:
            data_dict = {"Name": item["Name"], "CASN": item["CASN"]}
            names_db.append(data_dict)
        except IndexError:
            continue
            
    with open("namesdb.json", "w") as file:
        json.dump(names_db, file, indent=2) 
        
def parse_prdb() -> None:
    """
    Parse the `pr.ipd` file into a more structured json file.
    The `pr.ipd` is extracted from the ChemSep program database.
    """
    
    with open("pr.ipd", "r") as file:
        data = file.readlines()
        
    with open("namesdb.json", "r") as file:
        names_db = json.load(file)
    
    parsed_data = []
    for line in data:
        line_data = line.split()
        try:
            if line_data[0].split("-")[0].isnumeric():
                data_dict = {}
                
                data_dict["CASN_1"] = line_data[0]
                data_dict["CASN_2"] = line_data[1]
                
                for item in names_db:
                    if item["CASN"] == data_dict["CASN_1"]:
                        data_dict["Name_1"] = item["Name"]
                    elif item["CASN"] == data_dict["CASN_2"]:
                        data_dict["Name_2"] = item["Name"]
                    else:
                        continue
                
                data_dict["k12"] = line_data[2]
                data_dict["Comments"] = " ".join(line_data[3:])
                
                parsed_data.append(data_dict)
        except IndexError:
            continue
    
    with open("pripdb.json", "w") as file:
        json.dump(parsed_data, file, indent=2)
    
if __name__ == "__main__":
    parse_chemsepdb()
    parse_prdb()
    build_names_db()