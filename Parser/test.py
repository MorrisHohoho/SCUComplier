import subprocess
import os
import sys
from colorama import Fore, Back, Style


with os.scandir('./inputs') as inputs:
    #traverse all the inputs file to get syntax tree
    for input in inputs:

        print(Fore.BLACK+ "Now testing "+input.name)

        #set output_file path and name
        output_name = "./outputs/"+input.name
        output_name = output_name.replace(".cmm",".out")
        
        #call parser to get syntax tree
        with open(output_name,'w') as f:
            subprocess.run(["./code/parser",input.path],stdout=f,stderr=f)
        f.close()

        #get the expect output_file path and name
        ref_name = "./expects/"+input.name
        ref_name=ref_name.replace(".cmm",".exp")

        #Is the output all match the ref
        is_passed = True

        #check if the ref exists
        if  os.path.isfile(ref_name):

            #compare output_file and ref_file
            with open(ref_name,'r') as ref:
                with open(output_name,'r') as output:
                    lineno=1
                    ref_line = ref.readline()
                    output_line = output.readline()
                    while output_line:
                        if(ref_line==output_line):
                            ref_line=ref.readline()
                            output_line=output.readline()
                            lineno=lineno+1
                        else:
                            print(Fore.RED+"At line "+str(lineno)+" doesn't pass the test!")
                            is_passed=False
                            break
                if is_passed:
                    print(Fore.GREEN+output_name+" passed!")
                output.close()
            ref.close()
        else:
            print(Fore.YELLOW+ref_name+"doesn't exist!")      

        