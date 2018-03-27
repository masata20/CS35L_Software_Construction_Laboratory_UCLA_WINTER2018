#!/usr/bin/python

"""

Name: Masataka Mori
Select or reject lines common to two file

This program works as POSIX comm command,
detail description is in following URL:
http://pubs.opengroup.org/onlinepubs/9699919799/utilities/comm.html

"""
import sys
from optparse import OptionParser

class comm:

    ## initalizer
    def __init__(self, column1, column2, column3, unsort,
               file1, file2):
        ## if either file1 or file2 are "-"
        ## read all lines split with "\n"
        ## use readlines() function
        if file1 == "-":
            self.file1 = sys.stdin.readlines()
        else:
            f = open(file1, 'r')
            self.file1 = f.readlines()
            f.close()
            
        if file2 == "-":
            self.file2 = sys.stdin.readlines()
        else:
            f = open(file2, 'r')
            self.file2 = f.readlines()
            f.close()

        ## setter
        self.column1 = column1
        self.column2 = column2
        self.column3 = column3
        self.unsort = unsort

    def outputline(self, column, line):
        outputline = ""
        empty = ""
        if column == 1:
            if not self.column1:
                return empty
            return line
        if column == 2:
            if not self.column2:
                return empty
            if self.column1:
                return ("\t"+line)
            return line
        if column == 3:
            if not self.column3:
                return empty
            if self.column1 and self.column2:
                return ("\t\t"+line)
            if (not self.column1) and (not self.column2):
                return line
            return ("\t"+line)
        

    def result(self):
        ## if  unsorted do differenct operation
        if self.unsort:
            result_string = ""
            empty = ""
            edited_file2 = self.file2
            for line in self.file1:
                if line in self.file2:
                    result_string += self.outputline(3, line)
                    edited_file2.remove(line)
                else:
                    result_string += self.outputline(1,line)

            for line in edited_file2:
                result_string += self.outputline(2, line)

            return result_string
        else:
            file1_index = 0
            file2_index = 0
            file1_size = len(self.file1)
            file2_size = len(self.file2)
            result_string = ""

            while (file1_index < file1_size) and (file2_index < file2_size):
                #print file1_index
                #print file2_index
                line1 = self.file1[file1_index]
                line2 = self.file2[file2_index]
                if line1 in self.file2:
                    if file1_index == 0:
                        result_string += self.outputline(3, line1)
                        file1_index += 1
                        file2_index += 1
                    else:
                        if self.file1[file1_index-1] == line1:
                            result_string += self.outputline(1, line1)
                            file1_index += 1
                        else:
                            result_string += self.outputline(3, line1)
                            file1_index += 1
                            file2_index += 1 
                else:
                    if line1 > line2:
                        result_string += self.outputline(2, line2)
                        file2_index += 1
                    else:
                        result_string += self.outputline(1, line1)
                        if (file2_index < file2_size) and (line2 not in self.file1):
                            result_string += self.outputline(2, line2)
                            file2_index += 1
                        file1_index += 1
            
            
            while file1_index < file1_size:
                line1 = self.file1[file1_index]
                if line1 in self.file2:
                    if self.file1[file1_index-1] == line1:
                        result_string += self.outputline(1, line1)
                        file1_index += 1
                    else:
                        result_string += self.outputline(3, line1)
                        file1_index += 1
                else:
                    result_string += self.outputline(1, line1)
                    file1_index += 1
        
            while file2_index < file2_size:
                result_string += self.outputline(2, self.file2[file2_index])
                file2_index += 1
        
            return result_string
        
def main():
    version_msg = "%prog 2.0"                                                             
    usage_msg = """%prog [OPTION]... FILE1 FILE2                                                 
                                                                                          
Output comparison results between FILE1 and FILE2. If '-' used, it would use the standard input as FILE1, FILE2, and not both"""

    parser = OptionParser(version=version_msg,
                          usage=usage_msg)

    ## option -1
    ## Suppress the output column of lines unique to file1.
    parser.add_option("-1", "--column1",
                      action="store_false", dest="column1", default=True,
                      help="Suppress the output column of lines unique to file1")

    ## option -2
    ## Suppress the output column of lines unique to file2.
    parser.add_option("-2", "--column2",
                      action="store_false", dest="column2", default=True,
                      help="Suppress the output column of lines unique to file2")

    ## option -3
    ## Suppress the output column of lines duplicated in file1 and file2.
    parser.add_option("-3", "--column3",
                      action="store_false", dest="column3", default=True,
                      help="Suppress the output column of lines duplicated in file1 and file2.")

    ## option -u
    ## causes comm to work even if its inputs are not sorted
    parser.add_option("-u", "--unsort",
                      action="store_true", dest="unsort", default=False,
                      help="If its inputs are not sorted, sort them.")

    options, args = parser.parse_args(sys.argv[1:])

    ## check option -1
    try:
        column1 = bool(options.column1)
    except:
        parser.error("invalid COLUMN1: {0}".
                     format(options.column1))

    ## check option -2
    try:
        column2 = bool(options.column2)
    except:
        parser.error("invalid COLUMN2: {0}".
                     format(options.column2))

    ## check option -3
    try:
        column3 = bool(options.column3)
    except:
        parser.error("Invalid COLUMN3: {0}".
                     format(options.column3))

    ## check option -u
    try:
        unsort = bool(options.unsort)
    except:
        parser.error("Invalid UNSORT: {0}".
                     format(options.unsort))

    ## check operands
    if len(args) != 2:
        parser.error("wrong number of operands")

    ## both FILE1 and FILE2 cannot be the standard input
    if args[0]=="-" and args[1]=="-":
        parser.error("both FILE1 and FILE2 cannot be the standard input")


    file1 = args[0]
    file2 = args[1]

    try:
        generator = comm(column1, column2, column3, unsort, file1, file2)
    except OSError as err:                                                              
        parser.error("I/O error({0}): {1}".
                     format(err.errno, err.strerror)) 

    ## unavailble in python3
    #except IOError as (errno, strerror):
       # parser.error("I/O error({0}): {1}".
        #             format(errno, strerror))

    try:
        sys.stdout.write(generator.result())
    except:
        parser.error("ERROR occured during result function")

if __name__ == "__main__":
    main()
