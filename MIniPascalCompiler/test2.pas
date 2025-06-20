program Factorial;
var
    Counter, Factorial: integer;
begin
    Counter := 5;
    Factorial := 1;
    while Counter > 0 do
    begin
        Factorial := Factorial * Counter;
        Counter := Counter - 1;
    end;
    Write(Factorial);
end.#pragma once
