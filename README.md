# Morse code encoder/decoder

This program designed to encode ASCII text to morse code and to decode morse code back into ASCII text. The program can decode a series of characters supplied on the command line or take input from a file. Morse can also save its output to a file.

All morse characters and words must be delimited and by default the '/' character is used to delimit letters and a SPACE to separate words. However, any custom characters can be used.

Morse will decode letters of the alphabet, numbers, punctuations and some prosigns.

The following commands are available:

<DL>
<DT>-d<DT>
<DD>Decode the following morse sequence to text. A sequence of morse code characters consisting of dots (dits) '.' and dashes '-' (dahs) must be supplied. Each letter and word must be separated by a delimeter. The default delimeters are '/' for letter and a space for each word. Custom characters can be specified using the -s command.</DD>
<DT>-e</DT>
<DD>Encode the following sequence of ASCII text into Morse code. A sequence of characters must be supplied. Longer sequences containing multiple words should be enclosed in double quotes.</DD>
<DT>-m</DT>
<DD>Encode the specified file containing ASCII text into Morse code. A name of a file containing a sequence of ASCII characters must be supplied.</DD>
<DT>-o</DT>
<DD>Specify a file for output. Instead of sending output to STDIO, it will be stored in the specified file.</DD>
<DT>-s</DT>
<DD>Specify separator characters. Two separator characters must be supplied. If using a space, the characters must be enclosed in double quotes.</DD>
<DT>-t</DT>
<DD>Decode the specified file containing morse code into ASCII text. A name of a file containing Morse code needs to be supplied. The file must contain Morse code characters consisting of dots (dits) '.' and dashes '-' (dahs). Each letter and word must be separated by a delimeter. The default delimeters are '/' for letter and a space for each word group. Custom characters can be specified using the -s command.</DD>
</DL>

Examples:

<PRE>
morse -e "This is morse!"

morse -d "-/..../../... ../... --/---/.-./..././-.-.--"

morse -s " /" -d"- .... .. .../.. .../-- --- .-. ... . -.-.--"

morse -m example.txt -o /encoded.morse

morse -t example.morse -o /decoded.txt
</PRE>

