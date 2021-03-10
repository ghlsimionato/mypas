program Teste;

	{ Declarative scope }
	var i, j: integer;
	    x, y: real;
	    t, w: double;

	{ Declarative scope }
	procedure A(i: integer; var x: double);
		
		var abacaxi: integer;

		{ Imperative scope }
		begin
			abacaxi := j;

			writeln abacaxi

		end;

	{ Declarative scope }
	function fact(n: integer): integer;
		
		{ Imperative scope }
		begin
			if n > 2 then
				fact := n * fact(n - 1)
			else
				fact := n
		end;

        { Imperative scope }
	begin

		t := w * fact (j)
	end.

	O resto é comentário

