; Universidad Nacional Autónoma de México
; Facultad de Ingeniería
; Sistemas Expertos
; Proyecto final: Controlador de vuelo difuso
; Programador: Dorado Ibarra Sergio Alan

(defrule start
	(initial-fact)
=>
	(printout t "Enter the height [ft]: ")
	(bind ?height (read))
	(if (and(>= ?height 0)(<= ?height 1000)) then
		(assert (height ?height))
		(printout t "Enter the velocity [ft/s]: ")
		(bind ?velocity (read))
		(if (and(>= ?velocity -30)(<= ?velocity 30)) then
			(assert (velocity ?velocity))
		else
			(printout t "Wrong velocity. Please check." crlf)
			(assert (initial-fact)))
	else
		(printout t "Wrong height. Please check." crlf)
		(assert (initial-fact)))
)

;Fuzzification

(defrule height_fuzz
	?h <- (height ?height)
=>
	(retract ?h)
	
	;NZ
	(if (< ?height 500) then (bind ?height_nz (+ (* -0.002 ?height) 1)))
	(if (>= ?height 500) then (bind ?height_nz 0))
	(assert (height_nz ?height_nz))
	
	;S
	(if (< ?height 300) then (bind ?height_s (+ (* 0.002 ?height) 0.4)))
	(if (and (>= ?height 300) (< ?height 800)) then (bind ?height_s (+ (* -0.002 ?height) 1.6)))
	(if (>= ?height 800) then (bind ?height_s 0))
	(assert (height_s ?height_s))
	
	;M
	(if (<= ?height 300) then (bind ?height_m 0))
	(if (and (> ?height 300) (<= ?height 800)) then (bind ?height_m (- (* 0.002 ?height) 0.6)))
	(if (> ?height 800) then (bind ?height_m (+ (* -0.002 ?height) 2.6)))
	(assert (height_m ?height_m))
	
	;L
	(if (<= ?height 500) then (bind ?height_l 0))
	(if (> ?height 500) then (bind ?height_l (- (* 0.002 ?height) 1)))
	(assert (height_l ?height_l))
	
	(printout t "Height" crlf)
	(printout t "Height is NZ with mu(height) = " ?height_nz crlf)
	(printout t "Height is S with mu(height) = " ?height_s crlf)
	(printout t "Height is M with mu(height) = " ?height_m crlf)
	(printout t "Height is L with mu(height) = " ?height_l crlf)
)

(defrule velocity_fuzz
	?v <- (velocity ?velocity)
=>
	(retract ?v)
	
	;DL
	(if (< ?velocity -20) then (bind ?velocity_dl 1))
	(if (and(>= ?velocity -20)(< ?velocity -10)) then (bind ?velocity_dl (- (* -0.1 ?velocity) 1)))
	(if (>= ?velocity -10) then (bind ?velocity_dl 0))
	(assert (velocity_dl ?velocity_dl))
	
	;DS
	(if (and (< ?velocity -20)(<= -30 ?velocity)) then (bind ?velocity_ds 0))
	(if (and(<= -20 ?velocity)(< ?velocity -10)) then (bind ?velocity_ds (+ (* 0.1 ?velocity) 2)))
	(if (and(>= ?velocity -10)(< ?velocity 0)) then (bind ?velocity_ds (* -0.1 ?velocity)))
	(if (>= ?velocity 0) then (bind ?velocity_ds 0))
	(assert (velocity_ds ?velocity_ds))
	
	;Z
	(if (< ?velocity -10) then (bind ?velocity_z 0))
	(if (and(>= ?velocity -10)(< ?velocity 0)) then (bind ?velocity_z (+ (* 0.1 ?velocity) 1)))
	(if (and(>= ?velocity 0)(< ?velocity 10)) then (bind ?velocity_z (+ (* -0.1 ?velocity) 1)))
	(if (>= ?velocity 10) then (bind ?velocity_z 0))
	(assert (velocity_z ?velocity_z))

	;US
	(if (< ?velocity 0) then (bind ?velocity_us 0))
	(if (and(>= ?velocity 0)(< ?velocity 10)) then (bind ?velocity_us (* 0.1 ?velocity)))
	(if (and(>= ?velocity 10)(< ?velocity 20)) then (bind ?velocity_us (+ (* -0.1 ?velocity) 2)))
	(if (>= ?velocity 20) then (bind ?velocity_us 0))
	(assert (velocity_us ?velocity_us))

	;UL
	(if (< ?velocity 10) then (bind ?velocity_ul 0))
	(if (and(>= ?velocity 10)(< ?velocity 20)) then (bind ?velocity_ul (- (* 0.1 ?velocity) 1)))
	(if (>= ?velocity 20) then (bind ?velocity_ul 1))
	(assert (velocity_ul ?velocity_ul))

	(printout t "Velocity" crlf)
	(printout t "Velocity is DL with mu(velocity) = " ?velocity_dl crlf)
	(printout t "Velocity is DS with mu(velocity) = " ?velocity_ds crlf)
	(printout t "Velocity is Z with mu(velocity) = " ?velocity_z crlf)
	(printout t "Velocity is US with mu(velocity) = " ?velocity_us crlf)
	(printout t "Velocity is UL with mu(velocity) = " ?velocity_ul crlf)
)

;INFERENCE

(defrule HL_VDL "Height L Velocity DL"
	(height_l ?mu1)(velocity_dl ?mu2)
	?max <- (force_z_max ?force_z_max)
=>
	(bind ?force_z (min ?mu1 ?mu2))
	(if (> ?force_z ?force_z_max) then (retract ?max)(assert (force_z_max ?force_z)))
	(printout t "Force is Z with mu(force) = " ?force_z crlf)
)

(defrule HM_VDL "Height M Velocity DL"
	(height_m ?mu1)(velocity_dl ?mu2)
	?max <- (force_us_max ?force_us_max)
=>
	(bind ?force_us (min ?mu1 ?mu2))
	(if (> ?force_us ?force_us_max) then (retract ?max)(assert (force_us_max ?force_us)))
	(printout t "Force is US with mu(force) = " ?force_us crlf)
)

(defrule HS_VDL "Height S Velocity DL"
	(height_s ?mu1)(velocity_dl ?mu2)
	?max <- (force_ul_max ?force_ul_max)
=>
	(bind ?force_ul (min ?mu1 ?mu2))
	(if (> ?force_ul ?force_ul_max) then (retract ?max)(assert (force_ul_max ?force_ul)))
	(printout t "Force is UL with mu(force) = " ?force_ul crlf)
)

(defrule HNZ_VDL "Height NZ Velocity DL"
	(height_nz ?mu1)(velocity_dl ?mu2)
	?max <- (force_ul_max ?force_ul_max)
=>
	(bind ?force_ul (min ?mu1 ?mu2))
	(if (> ?force_ul ?force_ul_max) then (retract ?max)(assert (force_ul_max ?force_ul)))
	(printout t "Force is UL with mu(force) = " ?force_ul crlf)
)

(defrule HL_VDS "Height L Velocity DS"
	(height_l ?mu1)(velocity_ds ?mu2)
	?max <- (force_ds_max ?force_ds_max)
=>
	(bind ?force_ds (min ?mu1 ?mu2))
	(if (> ?force_ds ?force_ds_max) then (retract ?max)(assert (force_ds_max ?force_ds)))
	(printout t "Force is DS with mu(force) = " ?force_ds crlf)
)

(defrule HM_VDS "Height M Velocity DS"
	(height_m ?mu1)(velocity_ds ?mu2)
	?max <- (force_z_max ?force_z_max)
=>
	(bind ?force_z (min ?mu1 ?mu2))
	(if (> ?force_z ?force_z_max) then (retract ?max)(assert (force_z_max ?force_z)))
	(printout t "Force is Z with mu(force) = " ?force_z crlf)
)

(defrule HS_VDS "Height S Velocity DS"
	(height_s ?mu1)(velocity_ds ?mu2)
	?max <- (force_us_max ?force_us_max)
=>
	(bind ?force_us (min ?mu1 ?mu2))
	(if (> ?force_us ?force_us_max) then (retract ?max)(assert (force_us_max ?force_us)))
	(printout t "Force is US with mu(force) = " ?force_us crlf)
)

(defrule HNZ_VDS "Height NZ Velocity DS"
	(height_nz ?mu1)(velocity_ds ?mu2)
	?max <- (force_ul_max ?force_ul_max)
=>
	(bind ?force_ul (min ?mu1 ?mu2))
	(if (> ?force_ul ?force_ul_max) then (retract ?max)(assert (force_ul_max ?force_ul)))
	(printout t "Force is UL with mu(force) = " ?force_ul crlf)
)

(defrule HL_VZ "Height L Velocity Z"
	(height_l ?mu1)(velocity_z ?mu2)
	?max <- (force_dl_max ?force_dl_max)
=>
	(bind ?force_dl (min ?mu1 ?mu2))
	(if (> ?force_dl ?force_dl_max) then (retract ?max)(assert (force_dl_max ?force_dl)))
	(printout t "Force is DL with mu(force) = " ?force_dl crlf)
)

(defrule HM_VZ "Height M Velocity Z"
	(height_m ?mu1)(velocity_z ?mu2)
	?max <- (force_ds_max ?force_ds_max)
=>
	(bind ?force_ds (min ?mu1 ?mu2))
	(if (> ?force_ds ?force_ds_max) then (retract ?max)(assert (force_ds_max ?force_ds)))
	(printout t "Force is DS with mu(force) = " ?force_ds crlf)
)

(defrule HS_VZ "Height S Velocity Z"
	(height_s ?mu1)(velocity_z ?mu2)
	?max <- (force_z_max ?force_z_max)
=>
	(bind ?force_z (min ?mu1 ?mu2))
	(if (> ?force_z ?force_z_max) then (retract ?max)(assert (force_z_max ?force_z)))
	(printout t "Force is Z with mu(force) = " ?force_z crlf)
)

(defrule HNZ_VZ "Height NZ Velocity Z"
	(height_nz ?mu1)(velocity_z ?mu2)
	?max <- (force_z_max ?force_z_max)
=>
	(bind ?force_z (min ?mu1 ?mu2))
	(if (> ?force_z ?force_z_max) then (retract ?max)(assert (force_z_max ?force_z)))
	(printout t "Force is Z with mu(force) = " ?force_z crlf)
)

(defrule HL_VUS "Height L Velocity US"
	(height_l ?mu1)(velocity_us ?mu2)
	?max <- (force_dl_max ?force_dl_max)
=>
	(bind ?force_dl (min ?mu1 ?mu2))
	(if (> ?force_dl ?force_dl_max) then (retract ?max)(assert (force_dl_max ?force_dl)))
	(printout t "Force is DL with mu(force) = " ?force_dl crlf)
)

(defrule HM_VUS "Height M Velocity US"
	(height_m ?mu1)(velocity_us ?mu2)
	?max <- (force_dl_max ?force_dl_max)
=>
	(bind ?force_dl (min ?mu1 ?mu2))
	(if (> ?force_dl ?force_dl_max) then (retract ?max)(assert (force_dl_max ?force_dl)))
	(printout t "Force is DL with mu(force) = " ?force_dl crlf)
)

(defrule HS_VUS "Height S Velocity US"
	(height_s ?mu1)(velocity_us ?mu2)
	?max <- (force_ds_max ?force_ds_max)
=>
	(bind ?force_ds (min ?mu1 ?mu2))
	(if (> ?force_ds ?force_ds_max) then (retract ?max)(assert (force_ds_max ?force_ds)))
	(printout t "Force is DS with mu(force) = " ?force_ds crlf)
)

(defrule HNZ_VUS "Height NZ Velocity US"
	(height_nz ?mu1)(velocity_us ?mu2)
	?max <- (force_ds_max ?force_ds_max)
=>
	(bind ?force_ds (min ?mu1 ?mu2))
	(if (> ?force_ds ?force_ds_max) then (retract ?max)(assert (force_ds_max ?force_ds)))
	(printout t "Force is DS with mu(force) = " ?force_ds crlf)
)

(defrule HL_VUL "Height L Velocity UL"
	(height_l ?mu1)(velocity_ul ?mu2)
	?max <- (force_dl_max ?force_dl_max)
=>
	(bind ?force_dl (min ?mu1 ?mu2))
	(if (> ?force_dl ?force_dl_max) then (retract ?max)(assert (force_dl_max ?force_dl)))
	(printout t "Force is DL with mu(force) = " ?force_dl crlf)
)

(defrule HM_VUL "Height M Velocity UL"
	(height_m ?mu1)(velocity_ul ?mu2)
	?max <- (force_dl_max ?force_dl_max)
=>
	(bind ?force_dl (min ?mu1 ?mu2))
	(if (> ?force_dl ?force_dl_max) then (retract ?max)(assert (force_dl_max ?force_dl)))
	(printout t "Force is DL with mu(force) = " ?force_dl crlf)
)

(defrule HS_VUL "Height S Velocity UL"
	(height_s ?mu1)(velocity_ul ?mu2)
	?max <- (force_dl_max ?force_dl_max)
=>
	(bind ?force_dl (min ?mu1 ?mu2))
	(if (> ?force_dl ?force_dl_max) then (retract ?max)(assert (force_dl_max ?force_dl)))
	(printout t "Force is DL with mu(force) = " ?force_dl crlf)
)

(defrule HNZ_VUL "Height NZ Velocity UL"
	(height_nz ?mu1)(velocity_ul ?mu2)
	?max <- (force_ds_max ?force_ds_max)
=>
	(bind ?force_ds (min ?mu1 ?mu2))
	(if (> ?force_ds ?force_ds_max) then (retract ?max)(assert (force_ds_max ?force_ds)))
	(printout t "Force is DS with mu(force) = " ?force_ds crlf)
)

;COMPOSITION

(defrule composition
	(declare (salience -1))
	(force_dl_max ?dl)(force_ds_max ?ds)(force_z_max ?z)(force_us_max ?us)(force_ul_max ?ul)
	(test (or (> ?dl 0) (> ?ds 0) (> ?z 0) (> ?us 0) (> ?ul 0)))
=>
	(bind ?force_comp (max ?dl ?ds ?z ?us ?ul))
	(if (= ?force_comp ?dl) then 
		(bind ?type DL)
	else
		(if (= ?force_comp ?ds) then
			(bind ?type DS)
		else
			(if (= ?force_comp ?z) then
				(bind ?type Z)
			else
				(if (= ?force_comp ?us) then
					(bind ?type US)
				else
					(bind ?type US)
				)
			)
		)
	)
	(printout t "mu_f_dl_max = " ?dl crlf)
	(printout t "mu_f_ds_max = " ?ds crlf)
	(printout t "mu_f_z_max = " ?z crlf)
	(printout t "mu_f_us_max = " ?us crlf)
	(printout t "mu_f_ul_max = " ?ul crlf)
	(printout t "Force is " ?type " with mu(force) = " ?force_comp crlf)
)

;DEFUZZIFICATION

(defrule defuzz
	(declare (salience -2))
	(force_dl_max ?dl)(force_ds_max ?ds)(force_z_max ?z)(force_us_max ?us)(force_ul_max ?ul)
	(A1 ?A1)(A2 ?A2)(A3 ?A3)(A4 ?A4)(A5 ?A5)(A6 ?A6)(A7 ?A7)
	(c1 ?c1)(c2 ?c2)(c3 ?c3)(c4 ?c4)(c5 ?c5)(c6 ?c6)(c7 ?c7)
	(test (or (> ?dl 0) (> ?ds 0) (> ?z 0) (> ?us 0) (> ?ul 0)))
=>
	(bind ?a1 (* ?A1 ?dl))
	(bind ?a2 (* ?A2 ?dl))
	(bind ?a3 (* ?A3 ?ds))
	(bind ?a4 (* ?A4 ?z))
	(bind ?a5 (* ?A5 ?us))
	(bind ?a6 (* ?A6 ?ul))
	(bind ?a7 (* ?A7 ?ul))
	(bind ?a (+ ?a1 ?a2 ?a3 ?a4 ?a5 ?a6 ?a7))
	(bind ?c_A (+ (* ?a1 ?c1) (* ?a2 ?c2) (* ?a3 ?c3)(* ?a4 ?c4)(* ?a5 ?c5)(* ?a6 ?c7)))
	(bind ?force (/ ?c_A ?a))
	(printout t "Force is = " ?force crlf)
)

(deffacts initFacts
	(force_dl_max 0)
	(force_ds_max 0)
	(force_z_max 0)
	(force_us_max 0)
	(force_ul_max 0)
	(A1 10)
	(A2 5)
	(A3 10)
	(A4 10)
	(A5 10)
	(A6 5)
	(A7 10)
	(c1 -25)
	(c2 -16.6)
	(c3 -10)
	(c4 0)
	(c5 10)
	(c6 16.6)
	(c7 25)
)
