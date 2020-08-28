/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcharla <kcharla@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/13 10:18:44 by kcharla           #+#    #+#             */
/*   Updated: 2020/08/16 11:08:52 by kcharla          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mgx.h"

typedef struct		s_liner
{
	t_mgx			*mgx;
	t_int2			last_click;
	int				is_drawing;
}					t_liner;

int		liner_loop(void *liner_ptr)
{
	t_liner		*liner;
	t_rect		line;
	t_mgx		*mgx;

	if ((liner = (t_liner*)liner_ptr) == NULL)
		return (-1);
	if ((mgx = liner->mgx) == NULL)
		return (-1);
	if (mgx->win_active == NULL)
		return (-1);
	if (mgx->win_active->input.close)
	{
		ft_printf("pressed (X) button, exit...\n");
		exit(0);
	}
	if (mgx_mouse_clicked_right(mgx->win_active) && liner->is_drawing == TRUE)
	{
		mgx_buf_clear(mgx->win_active->bufs);
		liner->is_drawing = FALSE;
	}
	if (mgx_mouse_clicked_left(mgx->win_active) && liner->is_drawing == FALSE)
	{
		liner->last_click = mgx->win_active->input.mouse.pos;
		liner->is_drawing = TRUE;
	}
	else if (mgx_mouse_pressed_left(mgx->win_active) && liner->is_drawing == TRUE)
	{
		mgx_buf_clear(mgx->win_active->bufs);
		line.p1 = liner->last_click;
		line.p2 = mgx->win_active->input.mouse.pos;
		mgx_draw_line(mgx->win_active->bufs, line);
	}
	else if (mgx_mouse_released_left(mgx->win_active) && liner->is_drawing == TRUE)
	{
		mgx_buf_merge(mgx_get_win_buf(mgx->win_active, 2), mgx->win_active->bufs);
		mgx_buf_clear(mgx->win_active->bufs);
		liner->is_drawing = FALSE;
	}
	mgx_buf_merge(mgx->win_active->bufs, mgx_get_win_buf(mgx->win_active, 2));
	mgx_win_draw(mgx->win_active);
	return (0);
}

int				main()
{
	t_liner		liner;

	ft_bzero(&liner, sizeof(t_liner));
	if (mgx_init("liner.app", (t_size2){640, 480}, &(liner.mgx)))
		return (-1);
	if (mgx_win_buf_add(liner.mgx->win_active))
		return (-1);
	if (mgx_loop(liner.mgx, liner_loop, &liner))
		return (-1);
	return (0);
}