#include <gst/gst.h>

#include <gst/rtsp-server/rtsp-server.h>

#define DEFAULT_RTSP_PORT "554"

static char* password = "123456";

static char* port = (char *) DEFAULT_RTSP_PORT;

static char* command = (char *) "(  \
        rpicamsrc annotation-mode=date+time \
                  annotation-text-size=10 \
		  annotation-text-bg-colour=16777215 \
		  annotation-text-colour=12345 \
  		  roi-x=0.0 roi-w=1.0 \
                  roi-y=0.0 roi-h=1.0 \
                  sharpness=100 \
                  contrast=100 \
                  brightness=60 \
                  drc=0 \
                  preview=false \
                  bitrate=2000000 \
                  keyframe-interval=60 ! \
          video/x-h264, framerate=20/1, profile=high, width=640, height=360 ! \
          h264parse ! \
          rtph264pay name=pay0 pt=96 )";

int main (int argc, char *argv[]) {
  GMainLoop *loop;
  GstRTSPServer *server;
  GstRTSPMountPoints *mounts;
  GstRTSPMediaFactory *factory;
  GstRTSPAuth *auth;
  GstRTSPToken *token;
  gchar *basic;

  gst_init (&argc, &argv);
  loop = g_main_loop_new (NULL, FALSE);
  server = gst_rtsp_server_new ();
  g_object_set (server, "service", port, NULL);
  mounts = gst_rtsp_server_get_mount_points (server);
  factory = gst_rtsp_media_factory_new ();
  gst_rtsp_media_factory_set_launch (factory, command);
  gst_rtsp_mount_points_add_factory (mounts, "/stream", factory);
  g_object_unref (mounts);

  gst_rtsp_media_factory_add_role (factory, "user",
      GST_RTSP_PERM_MEDIA_FACTORY_ACCESS, G_TYPE_BOOLEAN, TRUE,
      GST_RTSP_PERM_MEDIA_FACTORY_CONSTRUCT, G_TYPE_BOOLEAN, TRUE, NULL);
  auth = gst_rtsp_auth_new ();
  token = gst_rtsp_token_new (GST_RTSP_TOKEN_MEDIA_FACTORY_ROLE, G_TYPE_STRING, "user", NULL);
  basic = gst_rtsp_auth_make_basic ("user", password);
  gst_rtsp_auth_add_basic (auth, basic, token);
  g_free (basic);
  gst_rtsp_token_unref (token);
  gst_rtsp_server_set_auth (server, auth);
  g_object_unref (auth);

  gst_rtsp_server_attach (server, NULL);

  g_print ("stream ready at rtsp://127.0.0.1:%s/stream\n", port);
  g_main_loop_run (loop);

  return 0;
}
