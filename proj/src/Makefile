PROG=proj

SRCS = menu.c game.c proj.c boat.c board.c interrupt_cicle.c kbd.c mouse.c rtc.c sprite.c timer.c graphic.c state_machine.c player.c

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic -D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
