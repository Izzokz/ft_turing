*This project has been created as part of the 42 curriculum by huvillat and kzhen-cl*
## Description

### Project Goal :

    The goal is seperated in two parts the progam part where we can read the json and the json where all the magic is done. We tried to reproduce a turing machine whose deterministic in way where what you gave can have undefined behavior because the json didn't have an instructions for this specific case. Now we can understand the input we can give need to respect certain rule of the json we use.

### Program Part :

    The program part were done by kzhen-cl 

### JSON Part :

    This part seperate five specific json :
    
        1. unary_add :

            In mathematics, a unary operation is an operation with only one operand, i.e. a single input. In the case of this project the json file can just reuse the exemple who is unary_sub. Yes you're replacing some line with other things to make it an unary addition.
    
        2. is_palindrome :

            is_palindrome is a specific case where the subject is not enough detailled but you can do it with just 1 and 0 no need to do like kzhen-cl do it and use the alphabet upper and lower.

        3. language_equal :

            For this one a big hint gonna be he work the same as is_palindrom because we have the same number of 1 and 0.
        
        4. even_language :

            And again same as the above. You reuse the previous one because in the big line he work the same.
    
        5. turing :
            
            This one is specific. In the subject they say "encoded as you fit".
            And for this specific sentence i reduce all the character in the json to a single line "b:[1z+=],s:[reaH],t:{r:[d1orw1cI,d+orw1cI,d=oew=cL],e:[d1oawzcI],a:[d=oHwzcL]}11+111=". If you want more detail go to "resource/json_machine_description/turing_resource.txt" here you have more detail for the how he was made.

    The JSON part were done by huvillat and kzhen-cl

## Instruction

### First Step :

    You can compile the program by using the terminal and typing at the base folder of the project "make". An executable named "ft_turing" appears do "./ft_turing --help" and follow the instruction given.

### Second Step :

    Now you know how you can compile and execut the programm if you need json file you can find them in "resource/json_machine_description/" here you all the file and more asked by the subject.

### Clean Step :
    
    Usual rule for the Makefile you can use in your terminal at the base folder "make clean" for just erasing object files (.o) and "make fclean" to do all the above plus erase the executable "ft_turing".

## Resources

    https://en.wikipedia.org/wiki/Unary_operation
    https://github.com/Davegamble/cjson
    https://www.cs.odu.edu/~zeil/cs390/f24/Public/turing-jflap/index.html