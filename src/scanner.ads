with Ada.Text_IO;   use Ada.Text_IO;

package Scanner is
   procedure Open (Path : String);
   procedure Close;
   procedure Advance;
   procedure Skip_Whitespace;
   function End_Of_File return Boolean;
   function Next return Character;
   function Peek return Character;
   function Peek_Next return Character;
   function Match (Item : Character) return Boolean;
   function Line return Positive;
   function Column return Positive;
private
   File : File_Type;
   Current_Character, Next_Character : Character;
   Current_Line, Current_Column : Positive := 1;
end Scanner;
