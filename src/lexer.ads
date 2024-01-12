package Lexer is
   type Token_Type is (End_Of_File, Error, Left_Parenthesis, Right_Parenthesis, Minus, Plus, Slash, Star, Number);
   type Token is record
      Category : Token_Type;
      Line     : Natural;
   end record;

   function Next return Token;
end Lexer;
