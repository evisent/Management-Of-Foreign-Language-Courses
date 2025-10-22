let stateData = {};
let languagesData = {};

const IS_INITIALIZED_KEY = 'languageSchoolInitialized';
const LANGUAGES = ['English', 'Spanish', 'German', 'Chinese', 'French', 'Arabian'];
const LEVELS = [1, 2, 3];
const INTENSITIES = [
    { name: 'Light', periods: 3 },           // Поддерживающее обучение
    { name: 'Standard', periods: 4 },        // Обычный  
    { name: 'Intensive', periods: 6 }        // интенсив
];

let selectedOption = null;

document.addEventListener('DOMContentLoaded', function() {
    const isInitialized = localStorage.getItem(IS_INITIALIZED_KEY);
    
    if (!isInitialized) {
        showInitModal();
    } else {
        closeInitModal();
    }
    
    getState();
    loadLanguages();
});

function showInitModal() {
    document.getElementById('initModal').style.display = 'flex';
}

function closeInitModal() {
    document.getElementById('initModal').style.display = 'none';
}

function selectOption(option) {
    selectedOption = option;
    
    // Убираем выделение со всех карточек
    document.querySelectorAll('.option-card').forEach(card => {
        card.classList.remove('selected');
    });
    
    // Добавляем выделение выбранной карточке
    event.currentTarget.classList.add('selected');
    
    if (option === 'auto') {
        createRandomStudents();
    } else if (option === 'manual') {
        showManualForm();
    }
}

function showManualForm() {
    document.querySelector('.init-options').style.display = 'none';
    document.getElementById('manualForm').style.display = 'block';
    generateManualStudentsForm();
}

function backToOptions() {
    document.getElementById('manualForm').style.display = 'none';
    document.querySelector('.init-options').style.display = 'grid';
    selectedOption = null;
    
    document.querySelectorAll('.option-card').forEach(card => {
        card.classList.remove('selected');
    });
}

function generateManualStudentsForm() {
    const grid = document.getElementById('manualStudentsGrid');
    grid.innerHTML = '';
    
    for (let i = 1; i <= 15; i++) {
        const studentCard = `
            <div class="student-form-card" data-student-id="${i}">
                <div class="student-header">
                    <div class="student-avatar-form">${i}</div>
                    <input type="text" class="student-name-input" placeholder="Имя студента" value="Студент ${i}">
                </div>
                
                <div class="languages-container" id="languages-${i}">
                    <div class="language-row">
                        <select class="form-select student-language">
                            <option value="">Выберите язык</option>
                            ${LANGUAGES.map(lang => `<option value="${lang}">${lang}</option>`).join('')}
                        </select>
                        <select class="form-select student-level">
                            <option value="">Уровень</option>
                            ${LEVELS.map(level => `<option value="${level}">${level}</option>`).join('')}
                        </select>
                        <select class="form-select student-intensity">
                            <option value="">Интенсивность</option>
                            ${INTENSITIES.map(intensity => 
                                `<option value="${intensity.name}">${intensity.name} (${intensity.periods} периодов)</option>`
                            ).join('')}
                        </select>
                        <button class="remove-language" onclick="removeLanguage(${i}, this)" ${i === 1 ? 'style="visibility:hidden;"' : ''}>×</button>
                    </div>
                </div>
                
                <button class="add-language" onclick="addLanguage(${i})">
                    + Добавить язык
                </button>
            </div>
        `;
        grid.innerHTML += studentCard;
    }
}

function addLanguage(studentId) {
    const container = document.getElementById(`languages-${studentId}`);
    const languageCount = container.querySelectorAll('.language-row').length;
    
    const languageRow = document.createElement('div');
    languageRow.className = 'language-row';
    languageRow.innerHTML = `
        <select class="form-select student-language">
            <option value="">Выберите язык</option>
            ${LANGUAGES.map(lang => `<option value="${lang}">${lang}</option>`).join('')}
        </select>
        <select class="form-select student-level">
            <option value="">Уровень</option>
            ${LEVELS.map(level => `<option value="${level}">${level}</option>`).join('')}
        </select>
        <select class="form-select student-intensity">
            <option value="">Интенсивность</option>
            ${INTENSITIES.map(intensity => 
                `<option value="${intensity.name}">${intensity.name} (${intensity.periods} периодов)</option>`
            ).join('')}
        </select>
        <button class="remove-language" onclick="removeLanguage(${studentId}, this)">×</button>
    `;
    
    container.appendChild(languageRow);
}

function removeLanguage(studentId, button) {
    const container = document.getElementById(`languages-${studentId}`);
    const rows = container.querySelectorAll('.language-row');
    
    if (rows.length > 1) {
        button.parentElement.remove();
    }
}

async function createRandomStudents() {
    try {
        addToOutput("🎲 Создание 15 случайных студентов...");
        
        const response = await fetch('/create_random_students', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            }
        });
        
        if (!response.ok) throw new Error('Ошибка сервера');
        
        const data = await response.json();
        addToOutput("✅ 15 случайных студентов созданы!");
        
        // Сохраняем состояние инициализации
        localStorage.setItem(IS_INITIALIZED_KEY, 'true');
        closeInitModal();
        loadLanguages();
        getState();
        
    } catch (error) {
        addToOutput("❌ Ошибка при создании случайных студентов: " + error);
    }
}

async function createManualStudents() {
    const studentCards = document.querySelectorAll('.student-form-card');
    const studentsData = [];
    
    for (let i = 0; i < studentCards.length; i++) {
        const card = studentCards[i];
        const name = card.querySelector('.student-name-input').value || `Студент ${i + 1}`;
        const languageRows = card.querySelectorAll('.language-row');
        
        const languages = [];
        let hasEmptyFields = false;
        
        for (const row of languageRows) {
            const language = row.querySelector('.student-language').value;
            const level = row.querySelector('.student-level').value;
            const intensity = row.querySelector('.student-intensity').value;
            
            if (!language || !level || !intensity) {
                hasEmptyFields = true;
                break;
            }
            
            languages.push({
                language: language,
                level: level,
                intensity: intensity
            });
        }
        
        if (hasEmptyFields) {
            alert(`Пожалуйста, заполните все поля для студента ${i + 1}`);
            return;
        }
        
        if (languages.length === 0) {
            alert(`Студент ${i + 1} должен изучать хотя бы один язык`);
            return;
        }
        
        studentsData.push({
            name: name,
            languages: languages
        });
    }
    
    try {
        addToOutput("🔄 Создание студентов...");
        
        const response = await fetch('/create_manual_students', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(studentsData)
        });
        
        if (!response.ok) throw new Error('Ошибка сервера');
        
        const data = await response.json();
        addToOutput(`✅ ${data.count} студентов успешно создано!`);
        
        // Сохраняем состояние инициализации
        localStorage.setItem(IS_INITIALIZED_KEY, 'true');
        closeInitModal();
        loadLanguages();
        getState();
        
    } catch (error) {
        addToOutput("❌ Ошибка при создании студентов: " + error);
    }
}

let isCreating = false;
async function createStudent() {
    if(isCreating){
        addToOutput("⏳ Подождите, создание уже выполняется...");
        return;
    }

    isCreating = true;
    const button = event.target;
    const originalText = button.innerHTML;
    
    button.innerHTML = '⏳ Следующий шаг...';
    button.disabled = true;

    try {
        const response = await fetch('/step', {
            method: 'POST'
        });
        const data = await response.json();
        getState();
        addToOutput("✅ Шаг симуляции выполнен!");
        loadLanguages(); // Это теперь загружает и группы и индивидуальных студентов
        
        if (!localStorage.getItem(IS_INITIALIZED_KEY)) {
            localStorage.setItem(IS_INITIALIZED_KEY, 'true');
        }
        
    } catch (error) {
        addToOutput("❌ Ошибка при выполнении шага: " + error);
    } finally{
        button.innerHTML = originalText;
        button.disabled = false;
        isCreating = false;
    }
}

// Также обновим другие функции которые должны обновлять интерфейс
async function deleteStudent() {
    try{
        const response = await fetch('/delete_student', {
            method: 'POST'
        })
        const data = await response.json();
        addToOutput("✅ Случайный студент покинул курс");
        loadLanguages(); // Обновляем интерфейс
        getState();
    } catch(error){
        addToOutput("❌ Ошибка при удалении студента: " + error);
    }
}

async function getState() {
    try {
        const response = await fetch('/state');
        stateData = await response.json();
        updateUI();
    } catch (error) {
        addToOutput("❌ Ошибка при получении состояния: " + error);
    }
}

async function loadLanguages() {
    try {
        const response = await fetch('/groups');
        const data = await response.json();
        languagesData = data.groups || [];
        displayLanguages(languagesData);
        
        // Загружаем индивидуальных студентов
        await loadIndividualStudents();
    } catch (error) {
        addToOutput("❌ Ошибка при загрузке групп: " + error);
    }
}

async function loadIndividualStudents() {
    try {
        const response = await fetch('/individual_students');
        const data = await response.json();
        displayIndividualStudents(data.individual_students || []);
    } catch (error) {
        addToOutput("❌ Ошибка при загрузке индивидуальных студентов: " + error);
    }
}

// Функция для загрузки индивидуальных студентов
async function loadIndividualStudents() {
    try {
        const response = await fetch('/individual_students');
        const data = await response.json();
        displayIndividualStudents(data.individual_students || []);
    } catch (error) {
        console.error("Ошибка при загрузке индивидуальных студентов: " + error);
    }
}

// Функция для отображения индивидуальных студентов
function displayIndividualStudents(individualStudents) {
    const individualList = document.getElementById('individualStudentsList');
    const individualCount = document.getElementById('individualCount');
    
    if (!individualList) return;
    
    individualCount.textContent = individualStudents.length;
    
    if (individualStudents.length === 0) {
        individualList.innerHTML = `
            <div class="empty-state">
                <div class="empty-icon">🎓</div>
                <h3>Нет индивидуальных студентов</h3>
                <p>Студенты из групп менее 5 человек будут переведены на индивидуальное обучение</p>
            </div>
        `;
        return;
    }
    
    individualList.innerHTML = individualStudents.map(student => `
        <div class="individual-student-card">
            <div class="student-avatar">${student.name ? student.name.charAt(0).toUpperCase() : '?'}</div>
            <div class="student-info">
                <div class="student-name">${student.name || 'Неизвестный'}</div>
                <div class="student-languages">
                    ${student.languages ? student.languages.map(lang => 
                        `<span class="language-tag">${lang.language} (Ур. ${lang.level}) - ${lang.price}₽</span>`
                    ).join('') : ''}
                </div>
                <div class="student-meta">
                    <span class="total-price">Всего: ${student.total_price || 0}₽</span>
                    ${student.languages && student.languages[0] ? 
                        `<span class="periods-left">${student.languages[0].periods_left} периодов осталось</span>` : ''}
                </div>
            </div>
        </div>
    `).join('');
}

// Обновим loadLanguages чтобы загружать и индивидуальных студентов
async function loadLanguages() {
    try {
        const response = await fetch('/groups');
        const data = await response.json();
        languagesData = data.groups || [];
        displayLanguages(languagesData);
        
        // Загружаем индивидуальных студентов
        await loadIndividualStudents();
    } catch (error) {
        addToOutput("❌ Ошибка при загрузке групп: " + error);
    }
}

function displayLanguages(groups) {
    const list = document.getElementById('languagesList');
    
    if (!groups || groups.length === 0) {
        list.innerHTML = `
            <div class="empty-state">
                <div class="empty-icon">🌍</div>
                <h3>Группы не найдены</h3>
                <p>Создайте студентов, чтобы сформировать группы</p>
            </div>
        `;
        return;
    }

    // Группируем по языкам
    const languagesMap = {};
    groups.forEach((group, index) => {
        const lang = group.language || 'Unknown';
        if (!languagesMap[lang]) {
            languagesMap[lang] = [];
        }
        // Добавляем уникальный идентификатор группы
        group.uniqueId = `${lang}-${group.level}-${group.intensity_name}-${index}`;
        languagesMap[lang].push(group);
    });

    list.innerHTML = Object.entries(languagesMap).map(([language, langGroups]) => {
        const totalStudents = langGroups.reduce((sum, group) => sum + (group.students?.length || 0), 0);
        const totalRevenue = langGroups.reduce((sum, group) => {
            const groupRevenue = (group.students || []).reduce((studentSum, student) => 
                studentSum + (student.price || 0), 0
            );
            return sum + groupRevenue;
        }, 0);
        
        return `
            <div class="language-item">
                <div class="language-header" onclick="toggleLanguage('${language}')">
                    <div class="language-name">
                        ${getLanguageFlag(language)} ${language}
                        <span style="font-size: 0.8em; color: var(--success); margin-left: 8px;">
                            💰 ${totalRevenue} ₽
                        </span>
                    </div>
                    <div style="display: flex; align-items: center; gap: 10px;">
                        <span class="language-count">${langGroups.length} групп</span>
                        <span class="language-toggle">▼</span>
                    </div>
                </div>
                <div class="groups-list" id="groups-${language}">
                    ${langGroups.map(group => {
                        const groupRevenue = (group.students || []).reduce((sum, student) => 
                            sum + (student.price || 0), 0
                        );
                        const avgPrice = group.students?.length ? 
                            Math.round(groupRevenue / group.students.length) : 0;
                            
                        return `
                        <div class="group-item" onclick="showGroupDetails('${group.uniqueId}')">
                            <div class="group-info">
                                <div>
                                    <div class="group-level">Уровень ${group.level || 1}</div>
                                    <div class="group-intensity">
                                        ${group.intensity_name || 'Unknown'} 
                                        <div style="color: var(--text-secondary); font-size: 0.8em; margin-top: 2px;">
                                            ${group.periods_left || 1} периодов всего
                                        </div>
                                        <div style="color: var(--success); font-size: 0.8em; margin-top: 2px;">
                                            💰 ${avgPrice} ₽/студ
                                        </div>
                                    </div>
                                </div>
                                <div class="group-students-count">
                                    <span class="student-count">${group.students?.length || 0}</span>
                                </div>
                            </div>
                        </div>
                    `}).join('')}
                </div>
            </div>
        `;
    }).join('');
}

function getLanguageFlag(language) {
    const flags = {
        'English': '🇬🇧',
        'Spanish': '🇪🇸', 
        'French': '🇫🇷',
        'German': '🇩🇪',
        'Chinese': '🇨🇳',
        'Arabian': '🇸🇦'
    };
    return flags[language] || '🌍';
}

function toggleLanguage(language) {
    const groupsList = document.getElementById(`groups-${language}`);
    const toggle = groupsList.previousElementSibling.querySelector('.language-toggle');
    
    groupsList.classList.toggle('expanded');
    toggle.classList.toggle('expanded');
}

function showGroupDetails(uniqueId) {
    const group = languagesData.find(g => g.uniqueId === uniqueId);

    if (!group) return;

    // Снимаем выделение со всех групп
    document.querySelectorAll('.group-item').forEach(item => {
        item.classList.remove('active');
    });

    // Выделяем выбранную группу
    event.target.closest('.group-item').classList.add('active');

    const details = document.getElementById('groupDetails');
    const totalRevenue = (group.students || []).reduce((sum, student) => sum + (student.price || 0), 0);
    const avgPrice = group.students?.length ? Math.round(totalRevenue / group.students.length) : 0;

    details.innerHTML = `
        <div class="details-card">
            <div class="details-header">
                <div class="details-title">
                    ${getLanguageFlag(group.language)} ${group.language} - Уровень ${group.level}
                </div>
                <div class="details-subtitle">
                    ${group.intensity_name} • ${group.students?.length || 0} студентов
                </div>
            </div>
            
            <div class="details-meta">
                <div class="meta-item">
                    <div class="meta-label">Интенсивность</div>
                    <div class="meta-value">${group.intensity_name}</div>
                </div>
                <div class="meta-item">
                    <div class="meta-label">Периоды обучения</div>
                    <div class="meta-value">${group.periods_left || 1} всего</div>
                </div>
                <div class="meta-item">
                    <div class="meta-label">Количество студентов</div>
                    <div class="meta-value">${group.students?.length || 0}</div>
                </div>
                <div class="meta-item">
                    <div class="meta-label">Средняя цена</div>
                    <div class="meta-value">${avgPrice} ₽</div>
                </div>
                <div class="meta-item">
                    <div class="meta-label">Общий доход</div>
                    <div class="meta-value">${totalRevenue} ₽</div>
                </div>
            </div>

            <div class="students-section">
                <h4>👥 Студенты в группе (${group.students?.length || 0})</h4>
                <div class="students-grid">
                    ${(group.students || []).map(student => `
                        <div class="student-card">
                            <div class="student-avatar">
                                ${student.name ? student.name.charAt(0).toUpperCase() : '?'}
                            </div>
                            <div class="student-info">
                                <div class="student-name">${student.name || 'Неизвестный'}</div>
                                <div class="student-details">
                                    <span class="price">${student.price || 0} ₽</span>
                                    <span class="periods">${student.student_periods_left || 0} периодов осталось</span>
                                </div>
                            </div>
                        </div>
                    `).join('')}
                </div>
            </div>
        </div>
    `;

    addToOutput(`📖 Открыта группа: ${group.language} - Уровень ${group.level} (${group.intensity_name})`);
}

function calculateAveragePrice(students) {
    if (!students || students.length === 0) return 0;
    
    const total = students.reduce((sum, student) => sum + (student.price || 0), 0);
    return Math.round(total / students.length);
}

function clearSelection() {
    document.querySelectorAll('.group-item').forEach(item => {
        item.classList.remove('active');
    });
    document.getElementById('groupDetails').innerHTML = `
        <div class="empty-state">
            <div class="empty-icon">👆</div>
            <h3>Выберите группу</h3>
            <p>Выберите язык или группу из списка слева чтобы увидеть детали</p>
        </div>
    `;
}

async function resetSchool() {
    if (!confirm("Вы уверены, что хотите очистить все данные? Это действие нельзя отменить.")) {
        return;
    }
    
    try {
        const response = await fetch('/reset', {
            method: 'POST'
        });
        const data = await response.json();
        addToOutput("✅ Система успешно сброшена!");
        
        // Удаляем состояние инициализации при сбросе
        localStorage.removeItem(IS_INITIALIZED_KEY);
        
        getState();
        clearSelection();
        loadLanguages();
        showInitModal(); // Показываем окно инициализации после сброса
        
    } catch (error) {
        addToOutput("❌ Ошибка при сбросе системы: " + error);
    }
}

function updateUI() {
    document.getElementById('studentsCount').textContent = stateData.students_count || 0;
    document.getElementById('groupsCount').textContent = stateData.groups_count || 0;
}

function clearOutput() {
    document.getElementById('output').textContent = "Журнал очищен\n";
    addToOutput("🗑️ Журнал событий очищен");
}

function addToOutput(message) {
    const output = document.getElementById('output');
    const timestamp = new Date().toLocaleTimeString();
    output.textContent += `[${timestamp}] ${message}\n`;
    output.scrollTop = output.scrollHeight;
}