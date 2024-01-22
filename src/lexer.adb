with Scanner;
with Ada.Characters.Latin_1;

package body Lexer is
   package Characters renames Ada.Characters.Latin_1;

   function Next return Token is
      C : Character;
   begin
      Scanner.Skip_Whitespace;
      Scanner.Mark;
      C := Scanner.Next;
      if Scanner.End_Of_File then
         return Token'(Kind => End_Of_File, Line => Scanner.Line);
      end if;
      return (case C is
         when '+' => Token'(Kind => Plus, Line => Scanner.Line),
         when '-' => (if Scanner.Match('-') then
               Line_Comment
            else
               Token'(Kind => Minus, Line => Scanner.Line)),
         when '*' => Token'(Kind => Star, Line => Scanner.Line),
         when '/' => Token'(Kind => Slash, Line => Scanner.Line),
         when '(' => Token'(Kind => Left_Parenthesis, Line => Scanner.Line),
         when ')' => Token'(Kind => Right_Parenthesis, Line => Scanner.Line),
         when Digit => Number,
         when others => Token'(Kind => Error, Line => Scanner.Line, Text => Scanner.Value));
   end Next;

   function Number return Token is
   begin
      while Scanner.Peek in Digit loop
         Scanner.Advance;
      end loop;
      return Token'
        (Kind => Number,
         Line => Scanner.Line,
         Text => Scanner.Value);
   end Number;

   function Line_Comment return Token is
      use Scanner;
      use Characters;
   begin
      Skip_Whitespace (Kind => Inline_Whitespace);
      Drop_Value; --  Dropthe "--" and any leading whitespace
      while not Is_Block_Whitespace (Peek) loop
         Advance;
      end loop;
      return Token'
        (Kind => Comment,
         Line => Line,
         Text => Value);
   end Line_Comment;
end Lexer;
