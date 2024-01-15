with Scanner;
with Ada.Strings.Unbounded;
use Ada.Strings.Unbounded;

package Lexer is
   type Token_Type is (
      End_Of_File, Error,
      Left_Parenthesis, Right_Parenthesis,
      Minus, Plus, Slash, Star,
      Number, Symbol,
      Comment
   );
   type Token is record
      Category : Token_Type;
      Line     : Natural;
      Text     : Unbounded_String;
   end record;

   function Next return Token;
private
   subtype Digit is Character range '0' .. '9';
   function Number return Token;
   function Line_Comment return Token;
end Lexer;
