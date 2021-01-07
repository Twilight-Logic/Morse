# Morse code encoder/decoder

This program designed to encode ASCII text to morse code and to decode morse code back into ASCII text.

The program can decode a series of characters on the command line or take input from a file. Morse can also
save its output to a file.

By default, the '/' character is used to delimit characters and a SPACE to separate words although any
custom characters can be used.

The following commands are available:

<DL>
<DT>-d</DT>
<DD>decode the following morse sequence to text. A sequence of morse code characters consisting of dots (dits) '.' and dashes '-' (dahs) must be supplied. Each letter and word must be separated by a delimeter. The default delimeters are '/' for letter and a space for each word. Custom characters can be specified using the -s command.</DD>
</DL>

-e	encode the follwoing sequence of ASCII text into morse code

	A sequence of characters must be supplied. Longer sequences containing multiple words should be enclosed
    in double quotes.

-m	encode the specified file containing ASCII text into morse code

	A name of a file containing a sequence of ASCII characters must be supplied.

-o	specify and output file

	Instead of being output to the STDIO, output will be stored in the specified file.

-s	specify separator characters

	the two separateor character must be supplied. If using a space, the characters must be enclosed in
	double quotes.

-t	decode the specified file containing morse code into ASCII text

	A name of a file containing morse code needs to be supplied. The file must contain morse code characters
	consisting of dots (dits) '.' and dashes '-' (dahs). Each letter and word must be separated by a delimeter.
	The default delimeters are '/' for letter and a space for each word. Custom characters can be specified
	using the -s command.

Examples:

morse -e "This is morse!"

morse -d "-/..../../... ../... --/---/.-./..././-.-.--"

morse -s " /" -d"- .... .. .../.. .../-- --- .-. ... . -.-.--"

morse -m example.txt -o /encoded.morse

morse -t example.morse -o /decoded.txt



