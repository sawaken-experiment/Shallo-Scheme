(load 'macro.scm)

(define repl (lambda (env)
	       (putc '#\>')
	       (putc '#\ ')
	       (let ((exp (read)) (catch-lambda (lambda (c) c)))
		 (display (catch catch-lambda 
				 (eval (cons 'top-level exp) env))))
	       (repl env)))

(repl (interaction-environment))
