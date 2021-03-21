 program Teste;

	{ Declarative scope }
	var i, j, o: integer;
	    x, y: real;
	    t, w: double;

	{ Declarative scope }
	procedure A(i: integer; var x: double);
		
		var abacaxi: integer;

		{ Imperative scope }
		begin
			abacaxi := j;


		end;

	{ Declarative scope }
	function fact(o: integer): integer;
		var n: integer;
		{ Imperative scope }
		begin
			if n > 2 then
				n := n * fact(n - 1)
			else
				n := n
		end;

        { Imperative scope }
	begin

		t := w * fact (j)
	end.

	O resto é comentário

