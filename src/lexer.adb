with Ada.Text_IO;
with Ada.Streams;
with Ada.Streams.Stream_IO;

package body Lexer is
    package IO renames Ada.Streams.Stream_IO;
    use type Ada.Streams.Stream_Element_Offset;

    procedure Open (File_Path : in String) is
    begin
        IO.Open (File, IO.In_File, File_Path);
        Ensure_Buffer_Has_Values;
    end;
    procedure Close is (IO.Close (File));

    procedure Ensure_Buffer_Has_Values is
    begin
        -- TODO: all the logic to make sure this is actually needed.
        IO.Read (File => File, Item => Buffer, Last => Last);
        Position := Buffer'First;
    end;

    procedure Advance is
    begin
        Position := Position + 1;
        Ensure_Buffer_Has_Values;
    end;

    procedure Next (Item : out Token) is
    begin
        null; -- TODO
    end;

    procedure Peek (Item : out Token) is
    begin
        null; -- TODO
    end;

    function Match (Category : Token_Type) return Boolean is
        Discard : Token;
    begin
        return Match (Category => Category, Item => Discard);
    end;

    function Match (Category : Token_Type;
                    Item : out Token)
                    return boolean is
    begin
        return False;
    end;
end;
