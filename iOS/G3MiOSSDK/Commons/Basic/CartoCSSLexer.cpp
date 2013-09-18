//
//  CartoCSSLexer.cpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 9/18/13.
//
//

#include "CartoCSSLexer.hpp"

#include "IStringUtils.hpp"
#include "CartoCSSTokens.hpp"


CartoCSSLexer::CartoCSSLexer(const std::string& source) :
_source(source),
_sourceSize(source.size()),
_cursor(0),
//  _previousToken(NULL),
//  _returnPreviousToken(false),
_su(IStringUtils::instance())
{
}

bool CartoCSSLexer::skipComments() {
  if (_cursor < _sourceSize-1) {
    const char c     = _source[_cursor];
    const char nextC = _source[_cursor+1];
    if (c == '/') {
      if (nextC == '/') {
        const int eolPosition = _su->indexOf(_source, "\n", _cursor+2);
        _cursor = (eolPosition < 0) ? _sourceSize : eolPosition + 1;
        return true;
      }
      else if (nextC == '*') {
        const int eocPosition = _su->indexOf(_source, "*/", _cursor+2);
        _cursor = (eocPosition < 0) ? _sourceSize : eocPosition + 3;
        return true;
      }
    }
  }
  return false;
}

bool CartoCSSLexer::skipBlanks() {
  const int cursor = _su->indexOfFirstNonBlank(_source, _cursor);
  if (cursor < 0) {
    _cursor = _sourceSize;
    return false;
  }
  const bool changedCursor = (cursor != _cursor);
  if (changedCursor) {
    _cursor = cursor;
  }
  return changedCursor;
}

void CartoCSSLexer::skipCommentsAndBlanks() {
  bool skipedBlanks;
  bool skipedCommment;
  do {
    skipedBlanks   = skipBlanks();
    skipedCommment = skipComments();
  }
  while (skipedBlanks || skipedCommment);
}


const CartoCSSToken* CartoCSSLexer::getNextToken() {
  //    if (_returnPreviousToken) {
  //      _returnPreviousToken = false;
  //      return _previousToken;
  //    }

  skipCommentsAndBlanks();
  if (_cursor >= _sourceSize) {
    return NULL;
  }

  CartoCSSToken* token;

  const char c = _source[_cursor];

  switch (c) {

    case '{': {
      token = new OpenBraceCartoCSSToken();
      _cursor++;
      break;
    }

    case '}': {
      token = new CloseBraceCartoCSSToken();
      _cursor++;
      break;
    }

    case '@': {
      token = new AtCartoCSSToken();
      _cursor++;
      break;
    }

    case ':': {
      token = new ColonCartoCSSToken();
      _cursor++;
      break;
    }

    case ';': {
      token = new SemicolonCartoCSSToken();
      _cursor++;
      break;
    }

    case '[': {
      const int closeBraquetPosition = _su->indexOf(_source, "]", _cursor+1);
      if (closeBraquetPosition < 0) {
        token = new ErrorCartoCSSToken("Unbalanced braquet", _cursor);
      }
      else {
        token = new ExpressionCartoCSSToken(_su->substring(_source, _cursor+1, closeBraquetPosition));
        _cursor = closeBraquetPosition+1;
      }
      break;
    }

      //      case '@': {
      //        const int semicolonPosition = _su->indexOf(_source, ";", _cursor+1);
      //        if (semicolonPosition < 0) {
      //          token = new ErrorCartoCSSToken("Can't find semicolon (;)", _cursor);
      //        }
      //        else {
      //          const int colonPosition = _su->indexOf(_source, ":", _cursor+1, semicolonPosition);
      //          if (colonPosition < 0) {
      //            token = new ErrorCartoCSSToken("Can't find colon (:)", _cursor);
      //          }
      //          else {
      //            const std::string name  = _su->trim( _su->substring(_source, _cursor+1, colonPosition) );
      //            const std::string value = _su->trim( _su->substring(_source, colonPosition+1, semicolonPosition) );
      //            token = new VariableCSSToken(name, value);
      //            _cursor = semicolonPosition+1;
      //          }
      //        }
      //        break;
      //      }

      //        OPEN_BRACE,           // {
      //        CLOSE_BRACE,          // }
      //        CONDITION_SELECTOR,
      //        VARIABLE,
      //        AT,                   // @
      //        COLON,                // :
      //        SEMICOLON,            // ;

    default: {
      const int cursor = _su->indexOfFirstNonChar(_source, "{}@:;", _cursor);
      if (cursor < 0) {
        token = new ErrorCartoCSSToken("Unknown token", _cursor);
      }
      else {
        const std::string str = _su->substring(_source, _cursor, cursor);
        token = new StringCartoCSSToken( str );
        _cursor = cursor;
      }

      break;
    }
  }

  //
  // http://www.w3.org/TR/CSS21/grammar.html
  //
  //   h          [0-9a-f]
  //   unicode    \\{h}{1,6}(\r\n|[ \t\r\n\f])?
  //   escape     {unicode}|\\[^\r\n\f0-9a-f]
  //   nonascii   [\240-\377]
  //   nmchar     [_a-z0-9-]|{nonascii}|{escape}
  //   name       {nmchar}+

  //    _previousToken = token;

  return token;
}