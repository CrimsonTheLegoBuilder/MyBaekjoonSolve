<template>
  <div>
    <!-- <p>{{ article }}</p> -->
    <p>{{ article.id }}</p>
    <p>{{ article.title }}</p>
    <p>{{ article.content }}</p>
    <RouterLink :to="{ name:'DetailView', params: { id: article.id } }">[Detail]</RouterLink>
    <hr>
  </div>
</template>

<script setup>
import { RouterLink } from 'vue-router';
defineProps({
  article: Object
})
</script>
