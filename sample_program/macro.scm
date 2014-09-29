(define-macro (define var_sym value)
  (list 'naive-define (list 'quote var_sym) value))

(define-macro (begin exp . rest)
  (cons 'body-form (cons exp rest)))

(define-macro (lambda params . body)
  (list 'naive-lambda (list 'quote params) (list 'quote (cons 'body-form body))))

(define-macro (let bindings . body)
  (cons (cons 'lambda (cons (map car bindings) body)) (map cadr bindings)))

(define-macro (quasiquote exp)
  (if (list? exp)
      (if (eq? 'unquote (car exp))
	  (car (cdr exp))
	  (if (eq? 'unquote-splicing (car exp))
	      (list 'splicing (car (cdr exp)))
	      (cons 'list (map (lambda (n) (list 'quasiquote n)) exp))))
      (list 'quote exp)))

(define-macro (set! symbol value)
  (list 'naive-set! (list 'quote symbol) value))

(define-macro (call/cc proc)
  (list proc '(get-nc)))

(define-macro (neq? a b)
  (list 'not (list 'eq? a b)))

(define map (lambda (f lst)
	      (if (null? lst)
		  lst
		  (cons (f (car lst)) (map f (cdr lst))))))

(define caar (lambda (a) (car (car a))))
(define cadr (lambda (a) (car (cdr a))))

