program ProcedureExample;
var
    x, y: integer;

procedure Swap(var a, b: integer);
var
    temp: integer;
begin
    temp := a;
    a := b;
    b := temp;
end;

begin
    x := 10;
    y := 20;
    Swap(x, y);
    writeln('x = ', x, ', y = ', y);
end.#pragma once
