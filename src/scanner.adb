with Ada.Characters.Handling;
with Ada.Characters.Latin_1;

package body Scanner is
   package IO renames Ada.Text_IO;
   package Characters renames Ada.Characters.Latin_1;

   procedure Open (Path : String) is
   begin
      IO.Open
        (File => File,
         Mode => IO.In_File,
         Name => Path);
      if not IO.End_Of_File (File) then
         IO.Get_Immediate (File => File, Item => Current_Character);
      else
         Current_Character := Characters.NUL;
      end if;
      if not IO.End_Of_File (File) then
         IO.Get_Immediate (File => File, Item => Next_Character);
      else
         Next_Character := Characters.NUL;
      end if;
   end Open;

   procedure Close is
   begin
      IO.Close (File);
   end Close;

   procedure Advance is
   begin

      IO.Put_Line
        (File => IO.Standard_Error,
         Item => "                " & Current_Character'Image & " (" & Current_Line'Image & "," & Current_Column'Image & " )");

      if Current_Character = Characters.LF then
         Current_Line := Current_Line + 1;
         Current_Column := 1;
      else
         Current_Column := Current_Column + 1;
      end if;

      Current_Character := Next_Character;
      if not IO.End_Of_File (File) then
         IO.Get_Immediate (File => File, Item => Next_Character);
      else
         Next_Character := Characters.NUL;
      end if;
   end Advance;

   procedure Skip_Whitespace is
      use Ada.Characters.Handling;
   begin
      loop
         exit when not (Is_Space (Peek) or else Is_Line_Terminator (Peek));
         Advance;
      end loop;
   end;

   function End_Of_File return Boolean is
     (IO.End_Of_File(File => File)
      and then Current_Character = Characters.NUL);

   function Next return Character is
      Value : Character := Current_Character;
   begin
      Advance;
      return Value;
   end Next;

   function Peek return Character is (Current_Character);
   function Peek_Next return Character is (Next_Character);

   function Match (Item : Character) return Boolean is
   begin
      if Peek = Item then
         Advance;
         return true;
      else
         return false;
      end if;
   end Match;

   function Line return Positive is (Current_Line);
   function Column return Positive is (Current_Column);
end Scanner;
