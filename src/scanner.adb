with Ada.Characters.Handling;
with Ada.Characters.Latin_1;
with Ada.Strings.Unbounded;

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
      use Ada.Strings.Unbounded;
   begin
      if Current_Character = Characters.LF then
         Current_Line := Current_Line + 1;
         Current_Column := 1;
      else
         Current_Column := Current_Column + 1;
      end if;

      if Current_Character /= Characters.NUL then
         Append (Token_Buffer, Current_Character);
      end if;

      Current_Character := Next_Character;
      if not IO.End_Of_File (File) then
         IO.Get_Immediate (File => File, Item => Next_Character);
      else
         Next_Character := Characters.NUL;
      end if;
   end Advance;

   procedure Skip_Whitespace (Kind : Whitespace_Kind := Any_Whitespace) is
      use Ada.Characters.Handling;
   begin
      loop
         exit when End_Of_File or not (case Kind is
            when Inline_Whitespace => Is_Space (Peek),
            when Block_Whitespace => Is_Block_Whitespace (Peek),
            when Any_Whitespace => Is_Space (Peek) or else Is_Block_Whitespace (Peek));
         Advance;
      end loop;
   end;

   procedure Mark is
      use Ada.Strings.Unbounded;
   begin
      Start_Line := Current_Line;
      Start_Column := Current_Column;
      Drop_Value;
   end Mark;

   procedure Drop_Value is
      use Ada.Strings.Unbounded;
   begin
      Token_Buffer := Null_Unbounded_String;
   end Drop_Value;

   function Value return Unbounded_String is (Token_Buffer);
   function End_Of_File return Boolean is (Current_Character = Characters.NUL);

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

   function Is_Block_Whitespace (C : Character) return Boolean is (case C is
      when Characters.LF => True,
      when Characters.CR => True,
      when Characters.NUL => True,
      when others => False);
end Scanner;
