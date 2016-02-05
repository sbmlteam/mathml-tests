/**
 * \file   createSyntacticTests.cpp
 * \brief  Create syntactic tests from the libsbml source validation files.
 * \author Lucian Smith
 * 
 * <!--------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2013-2014 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *     3. University of Heidelberg, Heidelberg, Germany
 *
 * Copyright (C) 2009-2013 jointly by the following organizations: 
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *  
 * Copyright (C) 2006-2008 by the California Institute of Technology,
 *     Pasadena, CA, USA 
 *  
 * Copyright (C) 2002-2005 jointly by the following organizations: 
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. Japan Science and Technology Agency, Japan
 * 
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ---------------------------------------------------------------------- -->*/

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>

#include <sbml/math/L3FormulaFormatter.h>
#include <sbml/math/L3Parser.h>
#include <sbml/math/MathML.h>


using namespace std;

void addAnnotationToMathML(ASTNode* astnode, const string& encoding, const string& annotstr)
{
  // create the annotation wrapping XMLNode
  XMLAttributes att = XMLAttributes();
  att.add("encoding", encoding);
  XMLToken ann_token1 = XMLToken(XMLTriple("annotation", "", ""), att);
  XMLNode *annotation = new XMLNode(ann_token1);

  // create an XML Text node using the same string as for AST
  XMLNode *infix = new XMLNode(annotstr);
  annotation->addChild(*infix);

  astnode->addSemanticsAnnotation(annotation);

}

string removeReturns(const string& input)
{
  string ret = input;
  size_t slashn = ret.find('\n');
  while (slashn != string::npos) {
    ret.replace(slashn, 1, "");
    slashn = ret.find('\n');
  }
  return ret;
}

void setupOrigInfix(vector<string>& infix)
{
  //infix.push_back("1e3");
#include "infixInput.h"
}

void setupJsbmlDifferences(map<string, string>& jsbml_differences)
{
  jsbml_differences.insert(make_pair("1.000000e+003", "1E3"));
  jsbml_differences.insert(make_pair("1.000000e-003", "1E-3"));
  jsbml_differences.insert(make_pair("1.000000e+003 unit", "1E3 unit"));
  jsbml_differences.insert(make_pair("1.000000e-003 unit", "1E-3 unit"));
}

void infixToMathML(vector<string>& infix, vector<string>& mathML_basic, vector<string>& mathML_annot)
{
  SBMLNamespaces sbmlns(3,1);
  for (size_t i=0; i<infix.size(); i++) {
    stringstream ss;
    XMLOutputStream xmlstream(ss);
    ASTNode* ast = SBML_parseL3Formula(infix[i].c_str());
    if (ast==NULL) {
      cerr << "Error:  unable to parse infix '" << infix[i] << "'" << endl;
    }
    writeMathML(ast, xmlstream, &sbmlns);
    mathML_basic.push_back(removeReturns(ss.str()));
    ss.clear();
    ss.str("");
    addAnnotationToMathML(ast, "infix-input", infix[i]);
    writeMathML(ast, xmlstream, &sbmlns);
    mathML_annot.push_back(ss.str());
    delete ast;
//    cout << ss.str() << endl;
  }
}

void mathMLToInfix(vector<string>& mathML, vector<string>& infix, const map<string, string>& jsbml_differences)
{
  SBMLNamespaces sbmlns(3,1);
  for (size_t i=0; i<mathML.size(); i++) {
    ASTNode* ast = readMathMLFromString(mathML[i].c_str());
    char* c_infix = SBML_formulaToL3String(ast);
    if (c_infix != NULL) {
      infix.push_back(c_infix);
    }
    else {
      cerr << "Error:  unable to parse mathML: " << endl << mathML[i] << endl;
      infix.push_back("");
    }
    map<string, string>::const_iterator difference = jsbml_differences.find(c_infix);
    if (difference == jsbml_differences.end()) {
      addAnnotationToMathML(ast, "infix-output", c_infix);
    }
    else {
      addAnnotationToMathML(ast, "infix-output-libsbml", c_infix);
      addAnnotationToMathML(ast, "infix-output-jsbml", difference->second);
    }
    stringstream ss;
    XMLOutputStream xmlstream(ss);
    writeMathML(ast, xmlstream, &sbmlns);
    mathML[i] = ss.str();
//    cout << c_infix << endl;
    free(c_infix);
  }
}

void writeMathMLToFiles(vector<string>& mathML)
{
  for (size_t i=0; i<mathML.size(); i++) {
    stringstream filename;
    filename << "mathML_test_";
    if (i<1000) {
      filename << "0";
    }
    if (i<100) {
      filename << "0";
    }
    if (i<10) {
      filename << "0";
    }
    filename << i << ".xml";
    ofstream file;
    file.open(filename.str().c_str());
    file << mathML[i] << endl;
    file.close();
  }
}

void writeInfixCompare(vector<string>& origInfix, vector<string>& mathML_basic, vector<string>& rtInfix, const map<string, string>& jsbml_differences)
{
  if (origInfix.size() != rtInfix.size()) {
    cerr << "Error:  infix input is not the same size as the round-tripped infix." << endl;
  }
  if (origInfix.size() != mathML_basic.size()) {
    cerr << "Error:  infix input is not the same size as the mathML output." << endl;
  }
  cout << "Infix input\tMathML output\tInfix output\tJSBML infix output (if different)" << endl;
  for (size_t i=0; i<origInfix.size(); i++) {
    cout << "'" << origInfix[i] << "\t";
    cout << mathML_basic[i] << "\t";
    cout << "'" << rtInfix[i];
    map<string, string>::const_iterator difference = jsbml_differences.find(rtInfix[i]);
    if (difference != jsbml_differences.end()) {
      cout << "\t'" << difference->second;
    }
    cout << endl;
  }
}


int
main (int argc, char* argv[])
{
  vector<string> origInfix, mathML_annot, mathML_basic, rtInfix;
  setupOrigInfix(origInfix);
  map<string, string> jsbml_differences;
  setupJsbmlDifferences(jsbml_differences);
  infixToMathML(origInfix, mathML_basic, mathML_annot);
  mathMLToInfix(mathML_annot, rtInfix, jsbml_differences);
  writeMathMLToFiles(mathML_annot);
  writeInfixCompare(origInfix, mathML_basic, rtInfix, jsbml_differences);
  return 0;
}

