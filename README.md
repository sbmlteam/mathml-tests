# mathml-tests
Tests for infix-to-mathML and mathML-to-infix for both libsbml and jsbml.

'summary.txt' and 'summary.xlsx' are spreadsheets with a summary of the tests.  The first column is the infix input used; the second is the MathML output; the third is the infix ouput (after being round-tripped), and the fourth column contains alternate round-trip infix output if jsbml instead of libsbml is used.

The dev/ directory contains a C++ program to create these tests, which works with libsbml.

The tests/ directory contains all the individual tests themselves.  Each test is an XML file containing MathML.  The infix used to create that MathML is included as an annotation with the encoding 'infix-input'::

    <annotation encoding="infix-input">a</annotation>

If both libsbml and jsbml produce the same infix output from this MathML, this will be annotated with the encoding 'infix-output':

    <annotation encoding="infix-output">a</annotation>

If libsbml and jsbml produce different (but correct) infix output from this MathML, each output will be included as a separate annotation, the first with the encoding 'infix-output-libsbml', and the second with the encoding 'infix-output-jsbml':

    <annotation encoding="infix-output-libsbml">1.000000e+003</annotation>
    <annotation encoding="infix-output-jsbml">1E3</annotation>

Note that libsbml contains options in its infix parser to change how it parses certain input.  None of these options are tested here, and the defaults are used throughout.  In particular, this means:

* 'log(x)' is always parsed as the natural log of x.
* Unary minuses ('-5', '--5', etc.) are not collapsed, and will always appear in the MathML with the '<minus>' element:

    <apply>
      <minus/>
      <cn type="integer"> 3 </cn>
    </apply>

instead of:

      <cn type="integer"> -3 </cn>

* Numerical values followed by a string are parsed as that value, with units of the string ("5 mL").
* The string 'avogadro' is parsed as the SMBL Level 3 csymbol 'avogadro'.
* All parsing of built-in functions and constants is case-insensitive.
