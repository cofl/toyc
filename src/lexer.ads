with Ada.Streams; use Ada.Streams;
with Ada.Streams.Stream_IO; use Ada.Streams.Stream_IO;

package Lexer is
    type Token_Type is (End_Of_File, Error);
    type Token is record
        Category    : Token_Type;
        Line        : Natural;
    end record;

    procedure Open (File_Path : in String);
    procedure Close;
    procedure Next (Item : out Token);
    procedure Peek (Item : out Token);
    function Match (Category : Token_Type) return Boolean;
    function Match (Category : Token_Type; Item : out Token) return Boolean;
private
    File    : File_Type;

    Extra   : Stream_Element;
    Buffer  : Stream_Element_Array (1 .. 1024);
    Last    : Stream_Element_Offset;
    Position    : Stream_Element_Offset := Buffer'First;

    procedure Advance;
    procedure Ensure_Buffer_Has_Values;
end Lexer;
