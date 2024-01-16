with Ada.Text_IO;
with Ada.Text_IO.Unbounded_IO;
with Ada.Command_Line;
with Lexer;
with Scanner;

procedure Toyc is
   package IO renames Ada.Text_IO;
   package CLI renames Ada.Command_Line;

   package Natural_IO is new IO.Integer_IO (Natural);
   use type Lexer.Token_Type;

   Token : Lexer.Token;
   Line  : Natural := 0;

   procedure Put_Token (Item : Lexer.Token) is
      use IO;
      use IO.Unbounded_IO;
      use Lexer;
   begin
      Put (Token.Category'Image);
      if Token.Category in Number | Symbol | Comment | Error then
         Put (": ");
         Put (Token.Text);
      end if;
   end Put_Token;
begin
   if CLI.Argument_Count /= 1 then
      IO.Put_Line (Item => "Usage: toyc <file>", File => IO.Standard_Error);
      CLI.Set_Exit_Status (Code => CLI.Failure);
      return;
   end if;

   Scanner.Open (CLI.Argument (1));
   loop
      Token := Lexer.Next;

      if Line /= Token.Line then
         Line := Token.Line;
         Natural_IO.Put (Item => Line, Width => 4);
         IO.Put (" ");
      else
         IO.Put ("   | ");
      end if;
      Put_Token (Token);
      IO.New_Line;

      exit when Token.Category = Lexer.End_Of_File;
   end loop;
   Scanner.Close;
end Toyc;
