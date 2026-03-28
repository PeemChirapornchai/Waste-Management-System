from django.contrib import admin
from django.urls import path
from django.conf import settings
from django.conf.urls.static import static
from detection import views

urlpatterns = [
    path('admin/', admin.site.urls),
    
    # The API for the ESP32
    path('api/waste/', views.process_waste, name='process_waste'),
    
    # The Frontend Dashboard
    path('', views.dashboard, name='dashboard'),
    
    # The API for the Dashboard to get text updates
    path('api/status/', views.get_latest_status, name='get_status'),
]

# This line allows Django to serve the saved images to your browser
if settings.DEBUG:
    urlpatterns += static(settings.MEDIA_URL, document_root=settings.MEDIA_ROOT)