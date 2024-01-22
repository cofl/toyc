with Ada.Text_IO;              use Ada.Text_IO;
with Ada.Text_IO.Unbounded_IO; use Ada.Text_IO.Unbounded_IO;
with Ada.Command_Line;         use Ada.Command_Line;
with Lexer;                    use Lexer;
with Scanner;

procedure Toyc is
   package Natural_IO is new Integer_IO (Natural);
   use Natural_IO;

   Item : Lexer.Token;
   Line : Natural := 0;

   procedure Put_Token is
   begin
      Put (Item.Kind'Image);
      if Item.Kind in Number | Symbol | Comment | Error then
         Put (": ");
         Put (Item.Text);
      end if;
   end Put_Token;
begin
   if Argument_Count /= 1 then
      Put_Line (Item => "Usage: toyc <file>", File => Standard_Error);
      Set_Exit_Status (Code => Failure);
      return;
   end if;

   Scanner.Open (Argument (1));
   loop
      Item := Next;

      if Line /= Item.Line then
         Line := Item.Line;

         Put (Item => Line, Width => 4);
         Put (" ");
      else
         Put ("   | ");
      end if;
      Put_Token;
      New_Line;

      exit when Item.Kind = Lexer.End_Of_File;
   end loop;
   Scanner.Close;
end Toyc;
