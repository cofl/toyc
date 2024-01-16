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
   type Token (Category : Token_Type := Error) is record
      Line     : Natural;
      case Category is
      when Number | Symbol | Comment | Error =>
         Text     : Unbounded_String;
      when others =>
         null;
      end case;
   end record;

   subtype End_Of_File_Token        is Token(End_Of_File);
   subtype Error_Token              is Token(Error);
   subtype Left_Parenthesis_Token   is Token(Left_Parenthesis);
   subtype Right_Parenthesis_Token  is Token(Right_Parenthesis);
   subtype Minus_Token              is Token(Minus);
   subtype Plus_Token               is Token(Plus);
   subtype Slash_Token              is Token(Slash);
   subtype Star_Token               is Token(Star);
   subtype Number_Token             is Token(Number);
   subtype Symbol_Token             is Token(Symbol);
   subtype Comment_Token            is Token(Comment);

   function Next return Token;
private
   subtype Digit is Character range '0' .. '9';
   function Number return Token;
   function Line_Comment return Token;
end Lexer;
