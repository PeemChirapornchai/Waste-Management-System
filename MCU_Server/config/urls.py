from django.urls import path
from detection import views

urlpatterns = [
    path('', views.dashboard, name='dashboard'),
]
