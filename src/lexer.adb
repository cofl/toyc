with Scanner;
with Ada.Characters.Latin_1;

package body Lexer is
   package Characters renames Ada.Characters.Latin_1;

   function Next return Token is
      C : Character;
   begin
      Scanner.Skip_Whitespace;
      C := Scanner.Next;
      return (case C is
         when '+' => Token'(Category => Plus, Line => Scanner.Line),
         when '-' => Token'(Category => Minus, Line => Scanner.Line),
         when '*' => Token'(Category => Star, Line => Scanner.Line),
         when '/' => Token'(Category => Slash, Line => Scanner.Line),
         when '(' => Token'(Category => Left_Parenthesis, Line => Scanner.Line),
         when ')' => Token'(Category => Right_Parenthesis, Line => Scanner.Line),
         when '0' .. '9' => Token'(Category => Number, Line => Scanner.Line),
         when Characters.NUL => Token'(Category => End_Of_File, Line => Scanner.Line),
         when others => Token'(Category => Error, Line => Scanner.Line));
   end Next;
end Lexer;
