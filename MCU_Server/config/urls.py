from django.contrib import admin
from django.urls import path
from detection.views import dashboard, upload_image, get_latest_image
from django.conf import settings
from django.conf.urls.static import static

urlpatterns = [
    path('admin/', admin.site.urls),
    path('', dashboard, name='dashboard'),
    path('upload', upload_image, name='upload'), # Access way for receiving images from the board.
    path('api/latest-image', get_latest_image, name='latest_image'), # The API allows the webpage to retrieve images.
]

if settings.DEBUG:
    urlpatterns += static(settings.STATIC_URL, document_root=settings.STATIC_ROOT)