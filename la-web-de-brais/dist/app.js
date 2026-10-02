const sites=[
  ['⛏️','Minecraft Wiki','Todo sobre bloques, biomas y criaturas','https://es.minecraft.wiki/','#67c76b'],
  ['🧱','Minecraft Dungeons II','Aventuras y secretos del juego','https://es.minecraft.wiki/w/Minecraft_Dungeons_II','#ffb85a'],
  ['👾','Stranger Things','Misterios del Otro Lado','https://enneperxp.vercel.app/stranger/','#fa6d7d'],
  ['🌌','Star Wars','Viaja por una galaxia lejana','https://enneperxp.vercel.app/starwars/','#7588ff'],
  ['🧪','Phineas y Ferb','Inventos para cada día de verano','https://enneperxp.vercel.app/phineas/','#74d9c4'],
  ['✈️','Cazas Star Wars','Naves rápidas para la misión','https://enneperxp.vercel.app/cazasstarwars/','#8aa8ff'],
  ['😄','Cuñado','Un rincón para reír','https://enneperxp.vercel.app/cuñado/','#ff9e64'],
  ['👑','Suegra','Otra aventura divertida','https://enneperxp.vercel.app/suegra/','#d998f7'],
  ['⏱️','Marty McFly','Regreso al Futuro','https://enneperxp.vercel.app/marty/','#5ec8ec']
];
const cards=document.querySelector('#cards'),side=document.querySelector('#side-links');
sites.forEach(([icon,name,description,url,color])=>{
  const card=document.createElement('a');card.className='card';card.href=url;card.target='_blank';card.rel='noopener noreferrer';card.style.setProperty('--accent',color);card.innerHTML=`<span class="icon">${icon}</span><b>${name}</b><small>${description}</small><i class="dot"></i>`;cards.append(card);
  const link=document.createElement('a');link.href=url;link.target='_blank';link.rel='noopener noreferrer';link.innerHTML=`<span>${icon}</span>${name}`;side.append(link);
});
document.querySelector('#menu').onclick=()=>document.querySelector('.sidebar').classList.toggle('open');
