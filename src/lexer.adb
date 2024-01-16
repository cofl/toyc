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
         return End_Of_File_Token'(Category => End_Of_File, Line => Scanner.Line);
      end if;
      return (case C is
         when '+' => Plus_Token'(Category => Plus, Line => Scanner.Line),
         when '-' => (if Scanner.Match('-') then
               Line_Comment
            else
               Minus_Token'(Category => Minus, Line => Scanner.Line)),
         when '*' => Star_Token'(Category => Star, Line => Scanner.Line),
         when '/' => Slash_Token'(Category => Slash, Line => Scanner.Line),
         when '(' => Left_Parenthesis_Token'(Category => Left_Parenthesis, Line => Scanner.Line),
         when ')' => Right_Parenthesis_Token'(Category => Right_Parenthesis, Line => Scanner.Line),
         when Digit => Number,
         when others => Error_Token'(Category => Error, Line => Scanner.Line, Text => Scanner.Value));
   end Next;

   function Number return Token is
   begin
      while Scanner.Peek in Digit loop
         Scanner.Advance;
      end loop;
      return Number_Token'
        (Category => Number,
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
      return Comment_Token'
        (Category => Comment,
         Line => Line,
         Text => Value);
   end Line_Comment;
end Lexer;
