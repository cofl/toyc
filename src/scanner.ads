with Ada.Text_IO;             use Ada.Text_IO;
with Ada.Strings.Unbounded;   use Ada.Strings.Unbounded;

package Scanner is
   type Whitespace_Kind is (Inline_Whitespace, Block_Whitespace, Any_Whitespace);

   procedure Open (Path : String);
   procedure Close;
   procedure Advance;
   procedure Skip_Whitespace (Kind : Whitespace_Kind := Any_Whitespace);

   procedure Mark;
   procedure Drop_Value;
   function Value return Unbounded_String;

   function End_Of_File return Boolean;
   function Next return Character;
   function Peek return Character;
   function Peek_Next return Character;
   function Match (Item : Character) return Boolean;
   function Line return Positive;
   function Column return Positive;

   function Is_Block_Whitespace (C : Character) return Boolean;
private
   File : File_Type;
   Current_Character, Next_Character : Character;
   Current_Line, Current_Column : Positive := 1;

   Token_Buffer : Unbounded_String := Null_Unbounded_String;
   Start_Line, Start_Column : Positive := 1;
end Scanner;
